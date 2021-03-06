// CatVilla.cpp : 定义应用程序的入口点。

#include "stdafx.h"
#include "CatVilla.h"
#include "MainApp.h"

class CCoInitialize {
public:
    CCoInitialize() : m_hr(CoInitialize(NULL)) { }
    ~CCoInitialize() { if (SUCCEEDED(m_hr)) CoUninitialize(); }
    operator HRESULT() const { return m_hr; }
    HRESULT m_hr;
};

int APIENTRY wWinMain(
    _In_     HINSTANCE,
    _In_opt_ HINSTANCE,
    _In_     LPWSTR,
    _In_     int)
{
    static CCoInitialize init;
    HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);
    if (SUCCEEDED(init) && SUCCEEDED(NumbFish::MainApp::GetInstance().Init()))
    {
        NumbFish::MainApp::GetInstance().MsgLoop();
    }
    return 0;
}
