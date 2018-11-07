#include "stdafx.h"
#include "Text.h"

namespace NumbFish
{
    Text::Text(
        const WCHAR*          fontName,
        int                   fontSize,
        Color                 fontColor,
        DWRITE_TEXT_ALIGNMENT textAlignment)
        :
        m_pIDWriteFactory(nullptr),
        m_pBrush(nullptr),
        m_pTextFormat(nullptr),
        m_fontName(const_cast<WCHAR*>(fontName)),
        m_fontSize(fontSize),
        m_fontColor(fontColor),
        m_textAlignment(textAlignment)
    {
        Creat();
    }

    Text::Text(const Text& text) :
        m_pIDWriteFactory(nullptr),
        m_pBrush(nullptr),
        m_pTextFormat(nullptr),
        m_fontName(text.m_fontName),
        m_fontSize(text.m_fontSize),
        m_fontColor(text.m_fontColor),
        m_textAlignment(text.m_textAlignment)
    {
        Creat();
    }

    Text::Text(Text&& text) :
        m_pIDWriteFactory(nullptr),
        m_pBrush(nullptr),
        m_pTextFormat(nullptr),
        m_fontName(nullptr),
        m_fontSize(0),
        m_fontColor(Color::Colorless),
        m_textAlignment(DWRITE_TEXT_ALIGNMENT_LEADING)
    {
        swap(text);
    }

    Text::~Text()
    {
        Release();
    }

    Text& Text::operator=(const Text& text)
    {
        Release();
        m_fontName = text.m_fontName;
        m_fontSize = text.m_fontSize;
        m_fontColor = text.m_fontColor;
        m_textAlignment = text.m_textAlignment;
        Creat();
        return *this;
    }

    Text& Text::operator=(Text&& text)
    {
        swap(text);
        return *this;
    }

    HRESULT Text::Creat()
    {
        Release();
        HRESULT hr{ S_OK };
        if (m_fontColor == Color::Colorless)
        {
            hr = E_FAIL;
        }
        if (SUCCEEDED(hr))
        {
            hr = DWriteCreateFactory(
                DWRITE_FACTORY_TYPE_SHARED,
                __uuidof(IDWriteFactory),
                reinterpret_cast<IUnknown**>(&m_pIDWriteFactory)
            );
        }
        if (SUCCEEDED(hr))
        {
            hr = m_pIDWriteFactory->CreateTextFormat(
                m_fontName,
                nullptr,
                DWRITE_FONT_WEIGHT_NORMAL,  // ±Ê»­ºñ¶È
                DWRITE_FONT_STYLE_NORMAL,   // ×ÖÌåÇãÐ±
                DWRITE_FONT_STRETCH_NORMAL, // ³¤¿í±È
                static_cast<FLOAT>(m_fontSize),
                L"", // locale
                &m_pTextFormat);
        }
        if (SUCCEEDED(hr))
        {
            m_pTextFormat->SetTextAlignment(m_textAlignment);
            m_pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_CHARACTER);
        }
        if (FAILED(hr))
        {
            Release();
            OutputMsg(L"[Error] Text ³õÊ¼»¯Ê§°Ü!");
        }
        return hr;
    }

    HRESULT Text::Draw(
        ID2D1RenderTarget*  pRenderTarget,
        const std::wstring& str,
        const Point&        point,
        const WidthHeight&  WH,
        const Color&        fontColor)
    {
        D2D1_SIZE_F renderTargetSize{ pRenderTarget->GetSize() };
        WidthHeight WHtmp{ WH };
        if (WHtmp.width == 0)
        {
            WHtmp.width = static_cast<int>(renderTargetSize.width);
        }
        if (WHtmp.height == 0)
        {
            WHtmp.height = static_cast<int>(renderTargetSize.height);
        }

        HRESULT hr{ S_OK };
        if (fontColor != Color::Colorless)
        {
            m_fontColor = fontColor;
        }
        if (m_fontColor == Color::Colorless)
        {
            hr = E_FAIL;
        }
        if (SUCCEEDED(hr) && (m_fontColor != fontColor || m_pBrush == nullptr))
        {
            SafeReleaseInterface(&m_pBrush);
            hr = pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(m_fontColor),
                &m_pBrush);
        }
        if (SUCCEEDED(hr))
        {
            pRenderTarget->DrawText(
                str.c_str(),
                str.size(),
                m_pTextFormat,
                D2D1::RectF(point.x * 1.0f, point.y * 1.0f, WHtmp.width * 1.0f, WHtmp.height * 1.0f),
                m_pBrush,
                D2D1_DRAW_TEXT_OPTIONS_NO_SNAP,
                DWRITE_MEASURING_MODE_GDI_CLASSIC);
        }
        if (FAILED(hr))
        {
            OutputMsg(L"[Error] Draw_Text Ê§°Ü!");
        }
        return hr;
    }

    void Text::Release()
    {
        SafeReleaseInterface(&m_pIDWriteFactory);
        SafeReleaseInterface(&m_pBrush);
        SafeReleaseInterface(&m_pTextFormat);
    }

    void Text::swap(Text& text) noexcept
    {
        using std::swap;
        swap(m_fontColor, text.m_fontColor);
        swap(m_fontName, text.m_fontName);
        swap(m_fontSize, text.m_fontSize);
        swap(m_pBrush, text.m_pBrush);
        swap(m_pIDWriteFactory, text.m_pIDWriteFactory);
        swap(m_pTextFormat, text.m_pTextFormat);
        swap(m_textAlignment, text.m_textAlignment);
    }
}
