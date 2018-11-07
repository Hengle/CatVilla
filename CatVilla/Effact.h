#pragma once
#include "stdafx.h"
#include "BoardObject.h"

namespace NumbFish
{
    class Effact : public BoardObject //!特效
    {
    public:
        using BoardObject::BoardObject;
        void Draw(                              //!绘制
            ID2D1RenderTarget*  pRenderTarget,  // 渲染目标
            const RowCol&       rc);            // 坐标
    };
}
