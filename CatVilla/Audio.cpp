#include "stdafx.h"
#include "Audio.h"

namespace NumbFish
{
    Audio::Audio(const LPCWSTR& fileName) :
        m_pXAudio2(nullptr),
        m_pMasterVoice(nullptr),
        m_pDataBuffer(nullptr),
        m_pSourceVoice(nullptr),
        m_fileName(fileName)
    {
        Creat();
    }

    Audio::Audio(const Audio& audio) :
        m_pXAudio2(nullptr),
        m_pMasterVoice(nullptr),
        m_pDataBuffer(nullptr),
        m_pSourceVoice(nullptr),
        m_fileName(audio.m_fileName)
    {
        Creat();
    }

    Audio::Audio(Audio&& audio) :
        m_pXAudio2(nullptr),
        m_pMasterVoice(nullptr),
        m_pDataBuffer(nullptr),
        m_pSourceVoice(nullptr),
        m_fileName(nullptr)
    {
        swap(audio);
    }

    Audio::~Audio()
    {
        Release();
    }

    Audio& Audio::operator=(const Audio& audio)
    {
        Release();
        m_fileName = audio.m_fileName;
        Creat();
        return *this;
    }

    Audio& Audio::operator=(Audio&& audio)
    {
        Release();
        swap(audio);
        return *this;
    }

    HRESULT Audio::Creat()
    {
        Release();
        HRESULT hr{ S_OK };
        if (SUCCEEDED(hr))
        {
            hr = XAudio2Create(&m_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
        }
        if (SUCCEEDED(hr))
        {
            hr = m_pXAudio2->CreateMasteringVoice(&m_pMasterVoice);
        }

        HANDLE hFile{};
        if (SUCCEEDED(hr))
        {
            hFile = CreateFile(
                m_fileName,
                GENERIC_READ,
                FILE_SHARE_READ,
                nullptr,
                OPEN_EXISTING,
                0,
                nullptr);
            if (INVALID_HANDLE_VALUE == hFile)
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
            }
        }
        if (SUCCEEDED(hr))
        {
            if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, nullptr, FILE_BEGIN))
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
            }
        }

        DWORD dwChunkPosition{};
        if (SUCCEEDED(hr))
        {
            //check the file type, should be fourccWAVE or 'XWMA'
            hr = FindChunk(hFile, fourccRIFF, m_dwChunkSize, dwChunkPosition);
        }

        DWORD filetype{};
        if (SUCCEEDED(hr))
        {
            hr = ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
        }
        if (SUCCEEDED(hr))
        {
            if (filetype != fourccWAVE)
            {
                hr = E_FAIL;
            }
        }
        if (SUCCEEDED(hr))
        {
            hr = FindChunk(hFile, fourccFMT, m_dwChunkSize, dwChunkPosition);
        }

        if (SUCCEEDED(hr))
        {
            hr = ReadChunkData(hFile, &m_waveFormat, m_dwChunkSize, dwChunkPosition);
        }
        if (SUCCEEDED(hr))
        {
            hr = FindChunk(hFile, fourccDATA, m_dwChunkSize, dwChunkPosition);
        }
        if (SUCCEEDED(hr))
        {
            SafeReleaseRawPointerArray(&m_pDataBuffer);
            m_pDataBuffer = new BYTE[m_dwChunkSize];
            hr = ReadChunkData(hFile, m_pDataBuffer, m_dwChunkSize, dwChunkPosition);
        }
        if (SUCCEEDED(hr))
        {
            hr = m_pXAudio2->CreateSourceVoice(&m_pSourceVoice, (WAVEFORMATEX*)&m_waveFormat, 0, 2.0f, &m_callback);
        }
        return hr;
    }

    HRESULT Audio::Restart(UINT32 LoopCount)
    {
        HRESULT hr{ S_OK };
        hr = End();
        if (SUCCEEDED(hr))
        {
            XAUDIO2_BUFFER buffer{};
            buffer.AudioBytes = m_dwChunkSize;
            buffer.pAudioData = m_pDataBuffer;
            buffer.Flags = XAUDIO2_END_OF_STREAM;
            buffer.LoopBegin = 0;
            buffer.LoopCount = LoopCount; // https://docs.microsoft.com/zh-cn/windows/desktop/api/xaudio2/ns-xaudio2-xaudio2_buffer
            buffer.LoopLength = LoopCount ? static_cast<UINT32>(m_dwChunkSize) / m_waveFormat.Format.nBlockAlign : 0;
            buffer.PlayLength = buffer.LoopLength;
            buffer.PlayBegin = 0;
            buffer.Flags = XAUDIO2_END_OF_STREAM;
            buffer.pContext = &m_callback;
            hr = m_pSourceVoice->SubmitSourceBuffer(&buffer);
        }
        if (SUCCEEDED(hr))
        {
            hr = m_pSourceVoice->Start(0);
        }
        return hr;
    }

    HRESULT Audio::Start()
    {
        return m_pSourceVoice->Start(0);
    }

    HRESULT Audio::Stop()
    {
        return m_pSourceVoice->Stop();
    }

    HRESULT Audio::End()
    {
        HRESULT hr{ S_OK };
        hr = m_pSourceVoice->Stop();
        if (SUCCEEDED(hr))
        {
            hr = m_pSourceVoice->FlushSourceBuffers();
        }
        return hr;
    }

    XAUDIO2_VOICE_STATE Audio::GetState() const
    {
        XAUDIO2_VOICE_STATE state;
        m_pSourceVoice->GetState(&state);
        return state;
    }

    void Audio::Release()
    {
        SafeReleaseVoice(&m_pSourceVoice);
        SafeReleaseVoice(&m_pMasterVoice);
        SafeReleaseInterface(&m_pXAudio2);
        SafeReleaseRawPointerArray(&m_pDataBuffer);
    }

    void Audio::swap(Audio& audio) noexcept
    {
        using std::swap;
        swap(m_pXAudio2, audio.m_pXAudio2);
        swap(m_pMasterVoice, audio.m_pMasterVoice);
        swap(m_pDataBuffer, audio.m_pDataBuffer);
        swap(m_pSourceVoice, audio.m_pSourceVoice);
        swap(m_fileName, audio.m_fileName);
        swap(m_callback, audio.m_callback);
        swap(m_dwChunkSize, audio.m_dwChunkSize);
        swap(m_waveFormat, audio.m_waveFormat);
    }

    HRESULT Audio::FindChunk(
        const HANDLE& hFile,
        DWORD  fourcc,
        DWORD& dwChunkSize,
        DWORD& dwChunkDataPosition)
    {
        HRESULT hr{ S_OK };
        if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, nullptr, FILE_BEGIN))
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        DWORD dwChunkType{};
        DWORD dwChunkDataSize{};
        DWORD dwRIFFDataSize{};
        DWORD dwFileType{};
        DWORD bytesRead{};
        DWORD dwOffset{};

        while (hr == S_OK)
        {
            DWORD dwRead;
            if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, nullptr))
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
            }

            if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr))
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
            }

            switch (dwChunkType)
            {
            case fourccRIFF:
                dwRIFFDataSize = dwChunkDataSize;
                dwChunkDataSize = 4;
                if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, nullptr))
                {
                    hr = HRESULT_FROM_WIN32(GetLastError());
                }
                break;

            default:
                if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, nullptr, FILE_CURRENT))
                {
                    return HRESULT_FROM_WIN32(GetLastError());
                }
            }

            dwOffset += sizeof(DWORD) * 2;

            if (dwChunkType == fourcc)
            {
                dwChunkSize = dwChunkDataSize;
                dwChunkDataPosition = dwOffset;
                return S_OK;
            }

            dwOffset += dwChunkDataSize;

            if (bytesRead >= dwRIFFDataSize) return S_FALSE;
        }
        return S_OK;
    }

    HRESULT Audio::ReadChunkData(
        const HANDLE& hFile,
        void* buffer,
        DWORD buffersize,
        DWORD bufferoffset)
    {
        if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, nullptr, FILE_BEGIN))
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }
        DWORD dwRead{};
        if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, nullptr))
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }
        else
        {
            return S_OK;
        }
    }
}
