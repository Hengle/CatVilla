#pragma once
#include "stdafx.h"
#include "BoardObject.h"

namespace NumbFish
{
    class Tile : public BoardObject // ��ש
    {
    public:
        using BoardObject::BoardObject;
        void Draw(                              //!����
            ID2D1RenderTarget*  pRenderTarget,  // ��ȾĿ��
            const RowCol&       rc);            // ����
    };

    typedef std::shared_ptr<Tile>           SpTile;
    typedef std::pair<std::string, SpTile> PspTile;
}
