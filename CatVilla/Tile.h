#pragma once
#include "stdafx.h"
#include "BoardObject.h"

namespace NumbFish
{
    class Tile : public BoardObject // 瓷砖
    {
    public:
        using BoardObject::BoardObject;
        void Draw(                              //!绘制
            ID2D1RenderTarget*  pRenderTarget,  // 渲染目标
            const RowCol&       rc);            // 坐标
    };

    typedef std::shared_ptr<Tile>           SpTile;
    typedef std::pair<std::string, SpTile> PspTile;
}
