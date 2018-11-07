#pragma once
#include "stdafx.h"
#include "BoardObject.h"

namespace NumbFish
{
    class BarInfo // 进度条信息
    {
    public:
        BarInfo(                        //!构造
            int     percent = 0,        // 百分比，默认0
            int     speed   = 10,       // 速度，默认10
            bool    start   = true);    // 启停，默认启
        int  m_percent;                 // 百分比
        int  m_speed;                   // 速度
        bool m_start;                   // 启停
    };

    class Bar : public BoardObject  //!进度条
    {
    public:
        using BoardObject::BoardObject;
        void Draw(                              //!绘制进度条框
            ID2D1RenderTarget*  pRenderTarget,  // 渲染目标
            const RowCol&       rc);            // 坐标
        void DrawProcess(                       //!绘制进度条进度
            ID2D1RenderTarget*  pRenderTarget,  // 渲染目标
            const RowCol&       rc,             // 坐标
            const BarInfo&      barInfo);       // 进度条信息
    };

    typedef std::shared_ptr<Bar> SpBar;
}
