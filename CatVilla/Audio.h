#pragma once
#include "stdafx.h"

namespace NumbFish
{
    /*todo ## ��Ƶ�ص��� ## */
    class AudioCallback : public IXAudio2VoiceCallback
    {
    public:
        HANDLE hBufferEndEvent; // ��Ƶ�����¼����

    public:
        AudioCallback() : hBufferEndEvent(CreateEvent(nullptr, false, false, nullptr)) { }
        ~AudioCallback() { CloseHandle(hBufferEndEvent); }

        // Called when the voice has just finished playing a contiguous audio stream.
        // �����ص�
        STDMETHOD_(void, OnStreamEnd) (THIS) { SetEvent(hBufferEndEvent); }

        // Unused methods are stubs
        // δʹ�ú���
        STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32) { }
        STDMETHOD_(void, OnVoiceProcessingPassEnd)   (THIS) { }
        STDMETHOD_(void, OnBufferStart)              (THIS_ void*) { }
        STDMETHOD_(void, OnBufferEnd)                (THIS_ void*) { }
        STDMETHOD_(void, OnLoopEnd)                  (THIS_ void*) { }
        STDMETHOD_(void, OnVoiceError)               (THIS_ void*, HRESULT) { }
    };

    /*todo ## ��Ƶ�� ## */
    class Audio
    {
    public:
        Audio() = delete; // ��ֹĬ��
        Audio(const LPCWSTR& fileName); // ���� ������Ƶ�ļ���
        Audio(const Audio&); // ����
        Audio(Audio&&); // �ƹ�
        ~Audio(); // ����

        Audio& operator=(const Audio&); // ��ֵ
        Audio& operator=(Audio&&); // �Ƹ�

        HRESULT Creat(); // ������Ƶ
        HRESULT Restart(UINT32 LoopCount = XAUDIO2_LOOP_INFINITE); // ������Ƶ����ʼλ��, Ĭ������ѭ������
        HRESULT Start(); // ��ʼ������Ƶ
        HRESULT Stop();  // ��ͣ������Ƶ
        HRESULT End();   // ֹͣ������Ƶ���ص���ʼλ��
        XAUDIO2_VOICE_STATE GetState() const; // ��ȡ��Ƶ״̬

    public:
        void Release(); // �ͷ�

    private:
        void swap(Audio&) noexcept; // ����

    private:
        HRESULT FindChunk( // Ѱ����Ƶ��
            const  HANDLE& hFile,
            DWORD  fourcc,
            DWORD& dwChunkSize,
            DWORD& dwChunkDataPosition);

        HRESULT ReadChunkData( // ��ȡ��Ƶ������
            const HANDLE& hFile,
            void* buffer,
            DWORD buffersize,
            DWORD bufferoffset);

    private:
        IXAudio2MasteringVoice* m_pMasterVoice;
        WAVEFORMATEXTENSIBLE    m_waveFormat;
        IXAudio2SourceVoice*    m_pSourceVoice;
        AudioCallback           m_callback;
        IXAudio2*               m_pXAudio2;
        LPCWSTR                 m_fileName;
        DWORD                   m_dwChunkSize;
        BYTE*                   m_pDataBuffer;
    };

    typedef std::shared_ptr<Audio>          SpAudio;
    typedef std::map<std::string, SpAudio> MspAudio;
}
