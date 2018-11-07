#include "stdafx.h"
#include "Bar.h"

namespace NumbFish
{
    BarInfo::BarInfo(int percent, int speed, bool start) : m_percent(percent), m_speed(speed), m_start(start) {}

    void Bar::Draw(ID2D1RenderTarget* pRenderTarget, const RowCol& rc)
    {
        m_pos.SetBoardPos(rc);
        if (m_imgs.size() <= 0 && m_imgs[0] == nullptr) { return; }
        m_imgs[0]->Draw(pRenderTarget, m_pos.GetScreenPos());
    }
    void Bar::DrawProcess(ID2D1RenderTarget* pRenderTarget, const RowCol& rc, const BarInfo& barInfo)
    {
        m_pos.SetBoardPos(rc);

        if (m_imgs.size() <= 1 && m_imgs[1] == nullptr) { return; }
        auto size = m_imgs[1]->GetSize();
        Point offset{ 0, 0 };
        if (m_pos.GetOffset().size() >= 1)
        {
            offset = m_pos.GetOffset()[0];
        }
        auto pos = m_pos.GetScreenPos() + offset;
        m_imgs[1]->DrawClip(pRenderTarget, pos, Point(), WidthHeight(
            static_cast<int>(size.width / 100 * barInfo.m_percent),
            static_cast<int>(size.height)));

        offset = Point(0, 0);
        if (m_pos.GetOffset().size() >= 2)
        {
            offset = m_pos.GetOffset()[1];
        }
        pos = m_pos.GetScreenPos() + offset;
        if (m_imgs.size() <= 2 && m_imgs[2] == nullptr) { return; }
        if (barInfo.m_percent >= 50)
        {
            m_imgs[2]->DrawClip(pRenderTarget, pos, Point(), WidthHeight(
                static_cast<int>(size.width / 100 * barInfo.m_percent),
                static_cast<int>(size.height)));
        }
    }
}
