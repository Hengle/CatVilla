#include "stdafx.h"
#include "MainApp.h"

namespace NumbFish
{
    int g_preWindowWidth{ 0 };
    int g_preWindowHeight{ 0 };
    bool g_fullScreen{ false };
    int g_cursorX{ 0 };
    int g_cursorY{ 0 };
    int g_debugX{ 0 };
    int g_debugY{ 0 };
    bool g_tileLine{ false };

    MainApp& MainApp::GetInstance()
    {
        static MainApp instance;
        return instance;
    }

    void MainApp::DealKey(HWND hWnd, int wParam)
    {
        UNUSED(hWnd);
        switch (wParam)
        {
        case 'W':
            --g_cursorY;
            OutputMsg("W");
            break;
        case 'A':
            --g_cursorX;
            OutputMsg("A");
            break;
        case 'S':
            ++g_cursorY;
            OutputMsg("S");
            break;
        case 'D':
            ++g_cursorX;
            OutputMsg("D");
            break;
        case 'J':
            m_mspAudio["Button"]->Restart(0);
            m_manager.GetCursor()->Request(CS_SELECT);
            OutputMsg("J");
            break;
        case 'K':
            m_mspAudio["Cancel"]->Restart(0);
            m_manager.GetCursor()->Request(CS_IDLE);
            OutputMsg("K");
            break;
        case 'L':
            OutputMsg("L");
            break;
        case VK_RETURN:
            OutputMsg("VK_RETURN");
            break;
        case VK_SPACE:
            OutputMsg("VK_SPACE");
            break;
        case VK_ESCAPE:
            OutputMsg("VK_ESCAPE");
            DestroyWindow(m_hWnd);
            break;
        }
    }

    MainApp::MainApp() :
        m_hWnd(nullptr),
        m_pD2DFactory(nullptr),
        m_pRenderTarget(nullptr)
    {
    }

    MainApp::~MainApp()
    {
        ChangeScreenResolution(g_preWindowWidth, g_preWindowHeight);
        ReleaseResources();
        SafeReleaseInterface(&m_pD2DFactory);
    }

    void MainApp::ReleaseResources()
    {
        SafeReleaseInterface(&m_pRenderTarget);
    }

    void MainApp::MsgLoop()
    {
        MSG msg{};
        while (GetMessage(&msg, nullptr, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void MainApp::GameLoop()
    {

    }

    void MainApp::ChangeScreenResolution(UINT width, UINT height)
    {
        DEVMODE lpDevMode{};
        lpDevMode.dmBitsPerPel = 32;
        lpDevMode.dmPelsWidth = width;
        lpDevMode.dmPelsHeight = height;
        lpDevMode.dmSize = sizeof(lpDevMode);
        lpDevMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
        LONG result{ ChangeDisplaySettings(&lpDevMode, 0) };
        if (result != DISP_CHANGE_SUCCESSFUL)
        {
            ChangeDisplaySettings(nullptr, 0);
        }
    }

    HRESULT MainApp::Init()
    {
        HRESULT hr{ InitDeviceIndependentResources() }; // �˴�����d2d����
        if (SUCCEEDED(hr))
        {
            WNDCLASSEX wcex{ sizeof(WNDCLASSEX) };
            wcex.style = CS_HREDRAW | CS_VREDRAW; // ���/�߶ȷ����仯���ƶ�ʱ���»��ƴ���
            wcex.lpfnWndProc = MainApp::WndProc; // ָ��WndProc
            wcex.cbClsExtra = 0;
            wcex.cbWndExtra = sizeof(LONG_PTR);
            wcex.hInstance = HINST_THISCOMPONENT;
            wcex.hbrBackground = nullptr;
            wcex.lpszMenuName = nullptr;
            wcex.hCursor = LoadCursor(nullptr, IDI_APPLICATION);
            wcex.lpszClassName = CLASS_NAME;

            RegisterClassEx(&wcex);

            FLOAT dpiX{}, dpiY{};
            m_pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);

            // �޸ķֱ���
            g_preWindowWidth = GetSystemMetrics(SM_CXSCREEN);
            g_preWindowHeight = GetSystemMetrics(SM_CYSCREEN);
            if (g_fullScreen) { ChangeScreenResolution(WINDOW_WIDTH, WINDOW_HEIGHT); }
            else { ChangeScreenResolution(g_preWindowWidth, g_preWindowHeight); }

            m_hWnd = CreateWindow(
                CLASS_NAME,
                WINDOW_NAME,
                WS_POPUP,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                static_cast<UINT>(WINDOW_WIDTH * dpiX / 96.f),  // ceil����ȡ��
                static_cast<UINT>(WINDOW_HEIGHT * dpiY / 96.f),
                nullptr,
                nullptr,
                HINST_THISCOMPONENT,
                this);
            hr = m_hWnd ? S_OK : E_FAIL;
            if (SUCCEEDED(hr))
            {
                SetWindowPos(
                    m_hWnd,
                    g_fullScreen ? HWND_TOPMOST : HWND_NOTOPMOST,
                    g_fullScreen ? 0 : (g_preWindowWidth - WINDOW_WIDTH) / 2,
                    g_fullScreen ? 0 : (g_preWindowHeight - WINDOW_HEIGHT) / 2,
                    WINDOW_WIDTH,
                    WINDOW_HEIGHT,
                    SWP_NOACTIVATE | SWP_DRAWFRAME);
                ShowWindow(m_hWnd, SW_SHOWNORMAL);
                UpdateWindow(m_hWnd);
            }
        }
        return hr;
    }

    LRESULT CALLBACK MainApp::WndProc(
        HWND   hWnd,
        UINT   msg,
        WPARAM wParam,
        LPARAM lParam)
    {
        LRESULT result{ 0 };
        static POINT pt{}, pe{};
        static RECT  rt{}, re{};
        static UINT width, height;

        if (msg == WM_CREATE) // �����׶�
        {
            LPCREATESTRUCT pcs{ (LPCREATESTRUCT)lParam };
            UNUSED(pcs);
            result = 1;
            SetTimer(hWnd, TIMER_ID, 1000 / FPS, NULL);
        }
        else
        {
            bool wasHandled{ false };
            static bool canKeyDownNow{ true };
            switch (msg)
            {
            case WM_SIZE:
                width = LOWORD(lParam);
                height = HIWORD(lParam);
                MainApp::GetInstance().Resize(width, height);
                result = 0;
                wasHandled = true;
                break;

            case WM_DISPLAYCHANGE:
                InvalidateRect(hWnd, nullptr, FALSE);
                result = 0;
                wasHandled = true;
                break;

            case WM_TIMER:
            case WM_PAINT:
                MainApp::GetInstance().Draw();
                ValidateRect(hWnd, nullptr);
                result = 0;
                wasHandled = true;
                break;

            case WM_KEYDOWN:
                if (canKeyDownNow == true)
                {
                    canKeyDownNow = false;
                    MainApp::GetInstance().DealKey(hWnd, wParam);
                }
                result = 0;
                wasHandled = true;
                break;

            case WM_KEYUP:
                canKeyDownNow = true;
                break;

            case WM_SYSKEYDOWN:
                if (wParam == VK_F10)
                {
                    OutputMsg(L"Key-F10");
                    g_fullScreen = !g_fullScreen;
                    MainApp::GetInstance().ChangeScreenResolution(
                        g_fullScreen ? WINDOW_WIDTH : g_preWindowWidth,
                        g_fullScreen ? WINDOW_HEIGHT : g_preWindowHeight);
                    SetWindowLongPtr(
                        MainApp::GetInstance().m_hWnd,
                        GWL_STYLE,
                        WS_POPUP);
                    SetWindowPos(
                        MainApp::GetInstance().m_hWnd,
                        g_fullScreen ? HWND_TOPMOST : HWND_NOTOPMOST,
                        g_fullScreen ? 0 : (g_preWindowWidth - WINDOW_WIDTH) / 2,
                        g_fullScreen ? 0 : (g_preWindowHeight - WINDOW_HEIGHT) / 2,
                        WINDOW_WIDTH,
                        WINDOW_HEIGHT,
                        SWP_NOACTIVATE | SWP_DRAWFRAME);
                    ShowWindow(MainApp::GetInstance().m_hWnd, SW_SHOW);
                }
                break;

            case WM_RBUTTONDOWN:
                SetCapture(MainApp::GetInstance().m_hWnd);         // ������겶��(��ֹ����ܳ�����ʧȥ����ȵ�)
                GetCursorPos(&pt);                      // ��ȡ�����ָ�뵱ǰλ��
                GetWindowRect(MainApp::GetInstance().m_hWnd, &rt); // ��ȡ����λ�����С
                re.right = rt.right - rt.left;          // ���洰�ڿ��
                re.bottom = rt.bottom - rt.top;         // ���洰�ڸ߶�
                break;

            case WM_RBUTTONUP:
                ReleaseCapture(); // �ͷ���겶�񣬻ָ�����״̬
                break;

            case WM_MOUSEMOVE:
                GetCursorPos(&pe); // ��ȡ���ָ�����λ��
                if (wParam == MK_RBUTTON) // ������Ҽ�����
                {
                    re.left = rt.left + (pe.x - pt.x); // �����µ�ˮƽλ��
                    re.top = rt.top + (pe.y - pt.y); // �����µĴ�ֱλ��
                    MoveWindow(MainApp::GetInstance().m_hWnd, re.left, re.top, re.right, re.bottom, true); // �ƶ�����
                }
                break;

            case WM_DESTROY:
                PostQuitMessage(0);
                result = 1;
                wasHandled = true;
                break;
            }
            if (!wasHandled)
            {
                result = DefWindowProc(hWnd, msg, wParam, lParam);
            }
        }
        return result;
    }

    HRESULT MainApp::InitDeviceIndependentResources()
    {
        HRESULT hr{ D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory) };

        /*todo ## ������������豸�޹ص���Դ ## */
        m_mspAudio["Music"] = std::make_shared<Audio>(L"Resource\\music\\music.wav");
        m_mspAudio["Button"] = std::make_shared<Audio>(L"Resource\\Sound\\Button.wav");
        m_mspAudio["Cancel"] = std::make_shared<Audio>(L"Resource\\Sound\\Cancel.wav");

        return hr;
    }

    HRESULT MainApp::InitDeviceResources()
    {
        HRESULT hr{ S_OK };
        if (m_pRenderTarget == nullptr)
        {
            RECT rc{};
            GetClientRect(m_hWnd, &rc); // ��ȡ�û������С
            D2D1_SIZE_U size{ D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top) };

            //!������ȾĿ��
            hr = m_pD2DFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(
                    m_hWnd,
                    size),
                &m_pRenderTarget);

            if (SUCCEEDED(hr))
            {
                /*todo ## ������������豸��ص���Դ���ص��Ƕ���Ҫ�õ�m_pRenderTarget ## */
                m_manager.StartManage(m_pRenderTarget);

                m_manager.SetCursor({
                    L"Resource\\image\\Cursors\\BlueCursor.png",
                    L"Resource\\image\\Tiles\\BlueLight.png" },
                    { { 63, 70 }, { { -25, 58 } } });
                auto cursor = m_manager.GetCursor();
                cursor->SetFloat(CF_VER);
                cursor->SetState(std::make_shared<CursorIdleState>(cursor));
                m_manager.AddTile("StoneBrick", {
                    L"Resource\\image\\Tiles\\Line\\StoneBrick.png" },
                    { { 38, 128 } });
                m_manager.AddCharacter("Erlang", {
                    L"Resource\\image\\Characters\\Erlang.png" },
                    { { 52, 93 } });
                m_manager.AddCharacter("JungleSwordsman", {
                    L"Resource\\image\\Characters\\JungleSwordsman.png" },
                    { { 54, 88 } });
                m_manager.AddCharacter("FengYao", {
                    L"Resource\\image\\Characters\\FengYao.png" },
                    { { 58, 90 } });
                m_manager.SetBar({
                    L"Resource\\image\\Bars\\Bar0.png",
                    L"Resource\\image\\Bars\\Bar1.png",
                    L"Resource\\image\\Bars\\Bar2.png" },
                    { { 54, 83 }, { { 2, 2 }, { 2, 2 } } });
                m_manager.SetBarToCharacter("Erlang");
                m_manager.SetBarToCharacter("JungleSwordsman");
                m_manager.SetBarToCharacter("FengYao");
                m_dialog = std::make_shared<Dialog>(m_pRenderTarget, FONT_NAME, FONT_SIZE, Color::Black, Paths{
                    L"Resource\\image\\Dialogs\\ACenter.png",
                    L"Resource\\image\\Dialogs\\ACorner.png",
                    L"Resource\\image\\Dialogs\\ASide.png" });

                m_manager.EndManage();
            }
        }
        return hr;
    }

    HRESULT MainApp::Draw()
    {
        HRESULT hr{ InitDeviceResources() }; // ������Դ
        if (SUCCEEDED(hr))
        {
            //!��ʼ����
            m_pRenderTarget->BeginDraw();
            m_pRenderTarget->SetTransform(D2D1::Matrix3x2F(IMAGE_MAGNIFICATION * 1.f, 0, 0, IMAGE_MAGNIFICATION * 1.f, 0, 0)); // ���þ���ı任
            m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DodgerBlue)); // ���û������ʱ����ɫ
            //!��ȡ��Ⱦ������δ�С
            static D2D1_SIZE_F renderTargetSize{ m_pRenderTarget->GetSize() };
            m_manager.StartManage(m_pRenderTarget);

            /*todo ## ������������� ## */
            static bool placeFlag = false;
            if (placeFlag == false)
            {
                placeFlag = true;
                for (int i = 0; i < 3; ++i)
                {
                    for (int j = 0; j < 8; ++j)
                    {
                        m_manager.Place({ i, j }, "StoneBrick");
                    }
                }
                m_manager.Place({ 0, 0 }, "Erlang");
                m_manager.Place({ 1, 0 }, "JungleSwordsman");
                m_manager.Place({ 2, 0 }, "FengYao");
            }

            /*todo ## ������������� ## */
            static int i = 0;
            ++i;
            i %= 100;
            m_manager.GetCharacter("Erlang")->SetBarInfo(i);
            m_manager.GetCharacter("JungleSwordsman")->SetBarInfo(i);
            m_manager.GetCharacter("FengYao")->SetBarInfo(i);

            m_manager.MoveCursor({ g_cursorY, g_cursorX });
            m_manager.Draw();

            m_dialog->Draw(m_pRenderTarget, L"test",
                { 0, WINDOW_HEIGHT / IMAGE_MAGNIFICATION - 50 },
                { static_cast<int>(renderTargetSize.width) / IMAGE_MAGNIFICATION, 50 });

            //!��������
            m_manager.EndManage();
            hr = m_pRenderTarget->EndDraw();

            if (hr == D2DERR_RECREATE_TARGET)
            {
                hr = S_OK;
                ReleaseResources();
            }
        }
        return hr;
    }

    void MainApp::Resize(UINT width, UINT height)
    {
        if (m_pRenderTarget)
        {
            m_pRenderTarget->Resize(D2D1::SizeU(width, height));
        }
    }
}
