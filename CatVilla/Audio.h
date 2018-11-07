#pragma once
#include "stdafx.h"

namespace NumbFish
{
    /*todo ## 音频回调类 ## */
    class AudioCallback : public IXAudio2VoiceCallback
    {
    public:
        HANDLE hBufferEndEvent; // 音频结束事件句柄

    public:
        AudioCallback() : hBufferEndEvent(CreateEvent(nullptr, false, false, nullptr)) { }
        ~AudioCallback() { CloseHandle(hBufferEndEvent); }

        // Called when the voice has just finished playing a contiguous audio stream.
        // 结束回调
        STDMETHOD_(void, OnStreamEnd) (THIS) { SetEvent(hBufferEndEvent); }

        // Unused methods are stubs
        // 未使用函数
        STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32) { }
        STDMETHOD_(void, OnVoiceProcessingPassEnd)   (THIS) { }
        STDMETHOD_(void, OnBufferStart)              (THIS_ void*) { }
        STDMETHOD_(void, OnBufferEnd)                (THIS_ void*) { }
        STDMETHOD_(void, OnLoopEnd)                  (THIS_ void*) { }
        STDMETHOD_(void, OnVoiceError)               (THIS_ void*, HRESULT) { }
    };

    /*todo ## 音频类 ## */
    class Audio
    {
    public:
        Audio() = delete; // 禁止默构
        Audio(const LPCWSTR& fileName); // 构造 传入音频文件名
        Audio(const Audio&); // 复构
        Audio(Audio&&); // 移构
        ~Audio(); // 析构

        Audio& operator=(const Audio&); // 赋值
        Audio& operator=(Audio&&); // 移赋

        HRESULT Creat(); // 创建音频
        HRESULT Restart(UINT32 LoopCount = XAUDIO2_LOOP_INFINITE); // 重置音频至开始位置, 默认无限循环播放
        HRESULT Start(); // 开始播放音频
        HRESULT Stop();  // 暂停播放音频
        HRESULT End();   // 停止播放音频，回到开始位置
        XAUDIO2_VOICE_STATE GetState() const; // 获取音频状态

    public:
        void Release(); // 释放

    private:
        void swap(Audio&) noexcept; // 交换

    private:
        HRESULT FindChunk( // 寻找音频块
            const  HANDLE& hFile,
            DWORD  fourcc,
            DWORD& dwChunkSize,
            DWORD& dwChunkDataPosition);

        HRESULT ReadChunkData( // 读取音频块数据
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
