#include "stdafx.h"
#include "Dialog.h"

namespace NumbFish
{
    Dialog::Dialog(SpText text, VspImg imgs) : m_text(text), m_imgs(imgs) { }
    Dialog::Dialog(
        ID2D1RenderTarget* pRenderTarget,
        std::wstring_view fontName,
        const size_t& fontSize,
        const Color& fontColor,
        const Paths& imgPaths)
        :
        m_text(std::make_shared<Text>(fontName.data(), fontSize, fontColor)),
        m_imgs()
    {
        if (imgPaths.size() < 3) { return; }
        for (int i = 0; i < 3; ++i)
        {
            m_imgs.push_back(std::make_shared<Img>(pRenderTarget, imgPaths[i].data()));
        }
    }
    void Dialog::Draw(ID2D1RenderTarget* pRenderTarget, std::wstring_view msg, const Point& pos, const WidthHeight& wh)
    {
        if (this == nullptr || m_imgs.size() < 3) { return; }
        auto imgCenterSize{ m_imgs[CENTER]->GetSize() };
        auto renderTargetSize{ pRenderTarget->GetSize() };
        auto[W, H] = wh;
        auto[CW, CH] = imgCenterSize;
        auto[PX, PY, _0, _1] = m_padding;
        auto[RW, RH] = renderTargetSize;
        m_imgs[CORNER]->Draw(pRenderTarget, pos);
        m_imgs[CORNER]->Draw(pRenderTarget, pos + Point(W, 0), 1.f, Transform(0, 1, 0));
        m_imgs[CORNER]->Draw(pRenderTarget, pos + Point(0, H), 1.f, Transform(0, 0, 1));
        m_imgs[CORNER]->Draw(pRenderTarget, pos + Point(W, H), 1.f, Transform(0, 1, 1));
        m_imgs[CENTER]->Draw(pRenderTarget, pos + m_padding, 1.f, Transform(0, 0, 0), {
            W / CW - 2 * PX,
            H / CH - 2 * PY });
        m_imgs[SIDE]->Draw(pRenderTarget, pos + Point(PX, 0), 1.f, Transform(0, 0, 0), {
            W / CW - 2 * PX,
            1.f });
        m_imgs[SIDE]->Draw(pRenderTarget, pos + Point(PX, H), 1.f, Transform(0, 0, 1), {
            W / CW - 2 * PX,
            1.f });
        m_imgs[SIDE]->Draw(pRenderTarget, pos + Point(W, PY), 1.f, Transform(1, 0, 0), {
            1.f,
            H / CH - 2 * PY });
        m_imgs[SIDE]->Draw(pRenderTarget, pos + Point(0, PY), 1.f, Transform(1, 1, 0), {
            1,
            H / CH - 2 * PY });
        m_text->Draw(pRenderTarget, msg.data(), pos + m_padding,
            WidthHeight(static_cast<int>(RW / IMAGE_MAGNIFICATION - pos.x - PX), 0));
    }
}