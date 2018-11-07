#include "stdafx.h"
#include "BoardObject.h"

namespace NumbFish
{
    BoardObject::BoardObject(const VspImg& imgs, const BoardPos& pos)
        : m_imgs(imgs), m_pos(pos) { }
    void BoardObject::SetPos(const BoardPos& pos) { m_pos = pos; }
    BoardPos BoardObject::GetPos() const { return m_pos; }
}
