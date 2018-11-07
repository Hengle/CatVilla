#include "stdafx.h"
#include "Effact.h"

namespace NumbFish
{
    void Effact::Draw(ID2D1RenderTarget* pRenderTarget, const RowCol& rc)
    {
        m_pos.SetBoardPos(rc);
        if (!m_imgs.size() && m_imgs[0] != nullptr) { return; }
        m_imgs[0]->Draw(pRenderTarget, m_pos.GetScreenPos());
    }
}
