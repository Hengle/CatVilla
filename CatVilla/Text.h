#pragma once
#include "stdafx.h"

namespace NumbFish
{
    /*todo ## 文本 ## */
    class Text
    {
    public:
        Text() = delete; // 拒绝默构
        Text( // 构造
            const WCHAR*          fontName,  // 字体名称
            int                   fontSize,  // 字体大小
            Color                 fontColor, // 字体颜色
            DWRITE_TEXT_ALIGNMENT textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING); // 字体对齐
        Text(const Text&); // 复构
        Text(Text&&); // 移构
        ~Text(); // 析构

        Text& operator=(const Text&); // 赋值
        Text& operator=(Text&&); // 移赋

        HRESULT Creat(); // 创建
        HRESULT Draw( // 绘制
            ID2D1RenderTarget*  pRenderTarget,      // 渲染目标
            const std::wstring& str,                // 文本
            const Point&        point,              // 绘制位置
            const WidthHeight&  WH = WidthHeight(), // 宽高
            const Color&        fontColor = Color::Colorless); // 颜色

    public:
        void Release(); // 释放

    private:
        void swap(Text&) noexcept; // 交换

    private:
        ID2D1SolidColorBrush* m_pBrush;
        IDWriteTextFormat*    m_pTextFormat;
        IDWriteFactory*       m_pIDWriteFactory;

    private:
        DWRITE_TEXT_ALIGNMENT m_textAlignment;
        WCHAR*                m_fontName;
        Color                 m_fontColor;
        int                   m_fontSize;
    };

    typedef std::shared_ptr<Text>          SpText;
    typedef std::vector<SpText>           VspText;
    typedef std::map<std::string, SpText> MspText;
}
