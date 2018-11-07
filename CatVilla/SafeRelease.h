#pragma once
#include "stdafx.h"

// ����ģ��������ԣ������Ͷ���Ҫ����ͬһ���ļ���

/*todo ## ���� ## */
template<class T> void SafeReleaseRawPointer(T** pp); // ��ȫ�ͷ�ԭʼָ��
template<class T> void SafeReleaseRawPointerArray(T** pp); // ��ȫ�ͷ�ԭʼָ������
template<class T> void SafeReleaseInterface(T** pp); // ��ȫ�ͷ�d2d�ӿ�
template<class T> void SafeReleaseVoice(T** pp); // ��ȫ�ͷ�xaudio2��Ƶ

/*todo ## ���� ## */
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
