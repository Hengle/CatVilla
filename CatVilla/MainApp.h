#pragma once
#include "stdafx.h"
#include "Text.h"
#include "Img.h"
#include "Audio.h"
#include "BoardObject.h"
#include "Tile.h"
#include "Character.h"
#include "BoardManager.h"
#include "Dialog.h"

namespace NumbFish
{
    class MainApp // µ¥ÀýÄ£Ê½
    {
    public:
        static MainApp& GetInstance();
        ~MainApp();
        HRESULT Init();
        void MsgLoop();
        void GameLoop();

    private:
        MainApp();
        
        MainApp& operator=(const MainApp&) = delete;
        MainApp& operator=(MainApp&&) = delete;

        HRESULT InitDeviceIndependentResources();
        HRESULT InitDeviceResources();
        void ReleaseResources();
        HRESULT Draw();
        void Resize(UINT width, UINT height);
        void DealKey(HWND hWnd, int wParam);
        void ChangeScreenResolution(UINT width, UINT height);
        static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        ID2D1HwndRenderTarget* m_pRenderTarget;
        ID2D1Factory* m_pD2DFactory;
        HWND m_hWnd;

        MspText     m_mspText;
        MspImg      m_mspImage;
        MspAudio    m_mspAudio;

        BoardManager m_manager;
        SpDialog     m_dialog;
    };
}
