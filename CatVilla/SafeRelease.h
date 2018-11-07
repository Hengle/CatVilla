#pragma once
#include "stdafx.h"

// 由于模板的特殊性，声明和定义要放在同一个文件里

/*todo ## 声明 ## */
template<class T> void SafeReleaseRawPointer(T** pp); // 安全释放原始指针
template<class T> void SafeReleaseRawPointerArray(T** pp); // 安全释放原始指针数组
template<class T> void SafeReleaseInterface(T** pp); // 安全释放d2d接口
template<class T> void SafeReleaseVoice(T** pp); // 安全释放xaudio2音频

/*todo ## 定义 ## */
template<class T>
void SafeReleaseRawPointer(T** pp)
{
    if (*pp != nullptr)
    {
        delete *pp;
        *pp = nullptr;
    }
}

template<class T>
void SafeReleaseRawPointerArray(T** pp)
{
    if (*pp != nullptr)
    {
        delete[] * pp;
        *pp = nullptr;
    }
}

template<class T>
void SafeReleaseInterface(T** pp)
{
    if (*pp != nullptr)
    {
        (*pp)->Release();
        *pp = nullptr;
    }
}

template<class T>
void SafeReleaseVoice(T** pp)
{
    if (*pp != nullptr)
    {
        (*pp)->DestroyVoice();
        *pp = nullptr;
    }
}
