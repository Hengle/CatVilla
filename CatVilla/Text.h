#pragma once
#include "stdafx.h"

namespace NumbFish
{
    /*todo ## �ı� ## */
    class Text
    {
    public:
        Text() = delete; // �ܾ�Ĭ��
        Text( // ����
            const WCHAR*          fontName,  // ��������
            int                   fontSize,  // �����С
            Color                 fontColor, // ������ɫ
            DWRITE_TEXT_ALIGNMENT textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING); // �������
        Text(const Text&); // ����
        Text(Text&&); // �ƹ�
        ~Text(); // ����

        Text& operator=(const Text&); // ��ֵ
        Text& operator=(Text&&); // �Ƹ�

        HRESULT Creat(); // ����
        HRESULT Draw( // ����
            ID2D1RenderTarget*  pRenderTarget,      // ��ȾĿ��
            const std::wstring& str,                // �ı�
            const Point&        point,              // ����λ��
            const WidthHeight&  WH = WidthHeight(), // ���
            const Color&        fontColor = Color::Colorless); // ��ɫ

    public:
        void Release(); // �ͷ�

    private:
        void swap(Text&) noexcept; // ����

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
