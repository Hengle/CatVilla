#pragma once
#include "stdafx.h"

namespace NumbFish
{
    /*todo ## 图片 ## */
    class Img
    {
    public:
        Img() = delete; // 禁止默构
        Img(ID2D1RenderTarget* pRenderTarget, const PCWSTR& uri); // 构造
        Img(const Img&); // 复构
        Img(Img&&); // 移构
        ~Img(); // 析构

        Img& operator=(const Img&); // 赋值
        Img& operator=(Img&&); // 移赋

        HRESULT Create(); // 创建
        HRESULT Draw( // 绘制
            ID2D1RenderTarget*        pRenderTarget, // 渲染目标
            const Point&              point,         // 绘制位置
            FLOAT                     opacity = 1.f, // 透明度
            const Transform&          transform = Transform(),       // 图像转换
            const WidthHeightPercent& WHPer = WidthHeightPercent()); // 图像百分比控制
        HRESULT DrawClip( // 绘制部分图像
            ID2D1RenderTarget*        pRenderTarget, // 渲染目标
            const Point&              point,         // 绘制位置
            const Point&              pointLeftTop,  // 在原图指定位置开始绘制
            const WidthHeight&        WH,            // 绘制图像的宽高
            FLOAT                     opacity = 1.f, // 透明度
            const Transform&          transform = Transform(),       // 图像转换
            const WidthHeightPercent& WHPer = WidthHeightPercent()); // 图像百分比控制
        D2D1_SIZE_F GetSize() const; // 获取图片尺寸

    public:
        void Release(); // 释放

    private:
        void swap(Img&) noexcept; // 交换

    private:
        HRESULT LoadBitmapFromResource( // 读取自资源
            ID2D1RenderTarget* pRenderTarget,
            const PCWSTR&      resourceName,
            const PCWSTR&      resourceType);

        HRESULT LoadBitmapFromFile(ID2D1RenderTarget* pRenderTarget); // 读取自文件

    private:
        IWICImagingFactory* m_pIWICFactory;
        ID2D1RenderTarget*  m_pRenderTarget;
        ID2D1Bitmap*        m_pBitmap;
        PCWSTR              m_uri;
    };

    typedef std::shared_ptr<Img>             SpImg;
    typedef std::vector<SpImg>              VspImg;
    typedef std::map<std::string, SpImg>    MspImg;
    typedef std::pair<std::string, SpImg>   PspImg;

    typedef std::wstring Path;
}
