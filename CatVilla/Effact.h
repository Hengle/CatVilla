#pragma once
#include "stdafx.h"
#include "BoardObject.h"

namespace NumbFish
{
    class Effact : public BoardObject //!��Ч
    {
    public:
        using BoardObject::BoardObject;
        void Draw(                              //!����
            ID2D1RenderTarget*  pRenderTarget,  // ��ȾĿ��
            const RowCol&       rc);            // ����
    };
}
