#pragma once
#include "stdafx.h"
#include "Img.h"
#include "Text.h"

namespace NumbFish
{
    class Dialog //!对话框
    {
    public:
        enum { CENTER = 0, CORNER = 1, SIDE = 2 };

        Dialog()                            = delete;   //!拒绝默构
        Dialog(const Dialog&)               = default;  //!默认复构
        Dialog(Dialog&&)                    = default;  //!默认移构
        ~Dialog()                           = default;  //!默认析构
        Dialog& operator=(const Dialog&)    = default;  //!默认赋值
        Dialog& operator=(Dialog&&)         = default;  //!默认移赋
        Dialog(                                         //!构造
            SpText text,                                // 字体
            VspImg imgs);                               // 对话框图像，分别为Center Corner Side
        Dialog(                                         //!构造
            ID2D1RenderTarget* pRenderTarget,           // 渲染目标
            std::wstring_view fontName,                 // 字体名称
            const size_t& fontSize,                     // 字体大小
            const Color& fontColor,                     // 字体颜色
            const Paths& imgPaths);                     // 图片路径
        void Draw(                                      //!绘制
            ID2D1RenderTarget* pRenderTarget,           // 渲染目标
            std::wstring_view msg,                      // 文字信息
            const Point& pos,                           // 坐标
            const WidthHeight& wh);                     // 长宽

    private:
        SpText m_text;                                  // 字体
        VspImg m_imgs;                                  // 图像
        Point m_padding{ 8, 8 };                        // 空隙
    };

    typedef std::shared_ptr<Dialog> SpDialog;
}
