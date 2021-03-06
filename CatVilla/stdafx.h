// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "XAudio2.lib")

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

// C/C++ RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <string_view>
#include <set>

// Direct2D
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

// XAudio2
#include <XAudio2.h>

// reference additional headers your program requires here
#include "BaseType.h"
#include "SafeRelease.h"

// debug
#include "OutputMsg.h"

#ifndef Assert
#if defined(DEBUG)||defined(_DEBUG)
#define Assert(b) do { if (!(b)) { OutputDebugStringA("Assert: " #b "\n"); } } while(0)
#else
#define Assert(b)
#endif // DEBUG || _DEBUG
#endif // Assert

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif // HINST_THISCOMPONENT

// XAudio2
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT  ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

#define UNUSED(para) (void)(para)

/*todo ## 重要常量表达式/宏定义 ## */
constexpr auto CLASS_NAME = L"Demo";
constexpr auto WINDOW_NAME = L"Demo";
constexpr auto IMAGE_MAGNIFICATION = 3;
constexpr auto WINDOW_WIDTH = 1280; // 428 * 3
constexpr auto WINDOW_HEIGHT = 720;  // 240 * 3
constexpr auto FONT_NAME = L"Zpix";
constexpr auto FONT_SIZE = 12;
constexpr auto TIMER_ID = 1;
constexpr auto FPS = 60;
