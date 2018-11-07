#pragma once
#include "stdafx.h"
#include "Img.h"
#include "Text.h"

namespace NumbFish
{
    class Dialog //!�Ի���
    {
    public:
        enum { CENTER = 0, CORNER = 1, SIDE = 2 };

        Dialog()                            = delete;   //!�ܾ�Ĭ��
        Dialog(const Dialog&)               = default;  //!Ĭ�ϸ���
        Dialog(Dialog&&)                    = default;  //!Ĭ���ƹ�
        ~Dialog()                           = default;  //!Ĭ������
        Dialog& operator=(const Dialog&)    = default;  //!Ĭ�ϸ�ֵ
        Dialog& operator=(Dialog&&)         = default;  //!Ĭ���Ƹ�
        Dialog(                                         //!����
            SpText text,                                // ����
            VspImg imgs);                               // �Ի���ͼ�񣬷ֱ�ΪCenter Corner Side
        Dialog(                                         //!����
            ID2D1RenderTarget* pRenderTarget,           // ��ȾĿ��
            std::wstring_view fontName,                 // ��������
            const size_t& fontSize,                     // �����С
            const Color& fontColor,                     // ������ɫ
            const Paths& imgPaths);                     // ͼƬ·��
        void Draw(                                      //!����
            ID2D1RenderTarget* pRenderTarget,           // ��ȾĿ��
            std::wstring_view msg,                      // ������Ϣ
            const Point& pos,                           // ����
            const WidthHeight& wh);                     // ����

    private:
        SpText m_text;                                  // ����
        VspImg m_imgs;                                  // ͼ��
        Point m_padding{ 8, 8 };                        // ��϶
    };

    typedef std::shared_ptr<Dialog> SpDialog;
}
