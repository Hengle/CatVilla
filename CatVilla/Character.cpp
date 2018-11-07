#include "stdafx.h"
#include "Character.h"

namespace NumbFish
{
    void Character::Draw(ID2D1RenderTarget* pRenderTarget, const RowCol& rc)
    {
        m_pos.SetBoardPos(rc);
        if (!m_imgs.size() && m_imgs[0] != nullptr) { return; }
        m_imgs[0]->Draw(pRenderTarget, m_pos.GetScreenPos());
        if (m_bar != nullptr)
        {
            m_bar->Draw(pRenderTarget, rc);
            m_bar->DrawProcess(pRenderTarget, rc, m_barInfo);
        }
    }
    void Character::SetBar(SpBar bar)
    {
        m_bar = bar;
    }
    void Character::RemoveBar()
    {
        m_bar = nullptr;
    }
    void Character::SetBarInfo(const BarInfo& barInfo)
    {
        m_barInfo = barInfo;
    }
    BarInfo Character::GetBarInfo() const
    {
        return m_barInfo;
    }
}
