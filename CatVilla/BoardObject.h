#pragma once
#include "stdafx.h"
#include "Img.h"

namespace NumbFish
{
    class BoardObject //!棋盘对象抽象基类
    {
    public:
        BoardObject()                               = delete;   //!拒绝默构
        BoardObject(const BoardObject&)             = default;  //!默认复构
        BoardObject(BoardObject&&)                  = default;  //!默认移构
        ~BoardObject()                              = default;  //!默认析构
        BoardObject& operator=(const BoardObject&)  = default;  //!默认赋值
        BoardObject& operator=(BoardObject&&)       = default;  //!默认移赋
        BoardObject(                                            //!构造
            const VspImg&   imgs,                               // 图像集合
            const BoardPos& pos);                               // 原始坐标
        void virtual    Draw(                                   //!绘制接口
            ID2D1RenderTarget*  pRenderTarget,                  // 渲染目标
            const RowCol&       rc) = 0;                        // 坐标
        void            SetPos  (const BoardPos& pos);          //!设置坐标
        BoardPos        GetPos  () const;                       //!获取坐标

    protected:
        VspImg      m_imgs;                                     // 图像集合
        BoardPos    m_pos;                                      // 坐标
    };
}
