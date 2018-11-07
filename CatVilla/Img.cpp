#include "stdafx.h"
#include "Img.h"

namespace NumbFish
{
    Img::Img(ID2D1RenderTarget* pRenderTarget, const PCWSTR& uri) :
        m_pRenderTarget(pRenderTarget),
        m_pIWICFactory(nullptr),
        m_pBitmap(nullptr),
        m_uri(uri)
    {
        Create();
    }

    Img::Img(const Img& img) :
        m_pRenderTarget(img.m_pRenderTarget),
        m_pIWICFactory(nullptr),
        m_pBitmap(nullptr),
        m_uri(img.m_uri)
    {
        Create();
    }

    Img::Img(Img&& img) :
        m_pRenderTarget(nullptr),
        m_pIWICFactory(nullptr),
        m_pBitmap(nullptr),
        m_uri(nullptr)
    {
        swap(img);
    }

    Img::~Img()
    {
        Release();
    }

    Img& Img::operator=(const Img& img)
    {
        Release();
        m_pRenderTarget = img.m_pRenderTarget;
        m_uri = img.m_uri;
        Create();
        return *this;
    }

    Img& Img::operator=(Img&& img)
    {
        Release();
        swap(img);
        return *this;
    }

    HRESULT Img::Create()
    {
        Release();
        HRESULT hr{ S_OK };
        if (SUCCEEDED(hr))
        {
            //!初始化IWIC工厂
            hr = CoCreateInstance(
                CLSID_WICImagingFactory,
                nullptr,
                CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(&m_pIWICFactory));
        }
        if (SUCCEEDED(hr))
        {
            //!读取图片资源
            hr = LoadBitmapFromFile(m_pRenderTarget);
        }
        if (FAILED(hr))
        {
            Release();
        }
        return hr;
    }

    HRESULT Img::Draw( // 绘制
        ID2D1RenderTarget*        pRenderTarget,
        const Point&              point,
        FLOAT                     opacity,
        const Transform&          transform,
        const WidthHeightPercent& WHPer)
    {
        HRESULT hr{ S_OK };
        D2D1_SIZE_F size{ GetSize() };
        D2D1::Matrix3x2F mat{
            IMAGE_MAGNIFICATION * 1.f, 0,
            0, IMAGE_MAGNIFICATION * 1.f,
            0, 0 };
        Point tmpPoint{ point };
        WidthHeightPercent tmpWHPer{ WHPer };
        if (transform.flipHor == true)
        {
            mat._11 *= -1;
            tmpPoint.x *= -1;
        }
        if (transform.flipVer == true)
        {
            mat._22 *= -1;
            tmpPoint.y *= -1;
        }
        if (transform.rotate90 == true)
        {
            mat._11 *= -1;
            std::swap(mat._11, mat._21);
            std::swap(mat._12, mat._22);
            tmpPoint.x += static_cast<int>(size.width - 1);
            tmpPoint.x *= -1;
            std::swap(tmpPoint.x, tmpPoint.y);
            std::swap(tmpWHPer.heightPer, tmpWHPer.widthPer);
        }
        pRenderTarget->SetTransform(mat);
        pRenderTarget->DrawBitmap(
            m_pBitmap,
            D2D1::RectF(
                tmpPoint.x * 1.f,
                tmpPoint.y * 1.f,
                tmpPoint.x + size.width  * tmpWHPer.widthPer,
                tmpPoint.y + size.height * tmpWHPer.heightPer),
            opacity,
            D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
        pRenderTarget->SetTransform(D2D1::Matrix3x2F(
            IMAGE_MAGNIFICATION * 1.f, 0,
            0, IMAGE_MAGNIFICATION * 1.f,
            0, 0));
        return hr;
    }

    HRESULT Img::DrawClip(
        ID2D1RenderTarget*        pRenderTarget,
        const Point&              point,
        const Point&              pointLeftTop,
        const WidthHeight&        WH,
        FLOAT                     opacity,
        const Transform&          transform,
        const WidthHeightPercent& WHPer)
    {
        HRESULT hr{ S_OK };
        ID2D1Layer* pLayer{ nullptr };
        hr = pRenderTarget->CreateLayer(nullptr, &pLayer);
        if (SUCCEEDED(hr))
        {
            pRenderTarget->PushLayer
            (
                D2D1::LayerParameters
                (
                    D2D1::RectF
                    (
                        point.x * 1.0f,
                        point.y * 1.0f,
                        point.x + WH.width  * WHPer.widthPer,
                        point.y + WH.height * WHPer.heightPer
                    )
                ),
                pLayer
            );
            Draw(
                pRenderTarget,
                Point(point.x - pointLeftTop.left, point.y - pointLeftTop.top),
                opacity,
                Transform(transform),
                WidthHeightPercent(WHPer));
            pRenderTarget->PopLayer();
        }
        SafeReleaseInterface(&pLayer);
        return hr;
    }

    D2D1_SIZE_F Img::GetSize() const
    {
        return m_pBitmap->GetSize();
    }

    void Img::Release()
    {
        SafeReleaseInterface(&m_pBitmap);
        SafeReleaseInterface(&m_pIWICFactory);
    }

    void Img::swap(Img& img) noexcept
    {
        using std::swap;
        swap(m_pRenderTarget, img.m_pRenderTarget);
        swap(m_pIWICFactory, img.m_pIWICFactory);
        swap(m_pBitmap, img.m_pBitmap);
        swap(m_uri, img.m_uri);
    }

    HRESULT Img::LoadBitmapFromResource(
        ID2D1RenderTarget*  pRenderTarget,
        const PCWSTR&       resourceName,
        const PCWSTR&       resourceType)
    {
        IWICBitmapDecoder*     pDecoder{ nullptr }; // 解码
        IWICBitmapFrameDecode* pSource{ nullptr }; // 资源
        IWICStream*            pStream{ nullptr }; // 流
        IWICFormatConverter*   pConverter{ nullptr }; // 格式转换

        HRSRC   imageResHandle{ nullptr }; // 图片资源句柄
        HGLOBAL imageResDataHandle{ nullptr }; // 图片资源数据句柄
        void*   pImageFile{ nullptr }; // 图片文件
        DWORD   imageFileSize{ 0 };    // 图片大小

        // 根据资源名称和类型寻找资源
        //!此处获得图片资源句柄
        imageResHandle = FindResourceW(HINST_THISCOMPONENT, resourceName, resourceType);
        HRESULT hr{ imageResHandle ? S_OK : E_FAIL };
        // 鬼畜的开始
        if (SUCCEEDED(hr))
        {
            // 根据图片资源句柄获得图片数据句柄
            //!此处获得图片资源数据句柄
            imageResDataHandle = LoadResource(HINST_THISCOMPONENT, imageResHandle);
            hr = imageResDataHandle ? S_OK : E_FAIL;
        }
        if (SUCCEEDED(hr))
        {
            // 锁定内存中的指定资源，即返回资源在内存中的地址，不须解锁操作
            //!此处获得图片文件
            pImageFile = LockResource(imageResDataHandle);
            hr = pImageFile ? S_OK : E_FAIL;
        }
        if (SUCCEEDED(hr))
        {
            //!此处获得图片文件大小
            imageFileSize = SizeofResource(HINST_THISCOMPONENT, imageResHandle);
            hr = imageFileSize ? S_OK : E_FAIL;
        }
        if (SUCCEEDED(hr))
        {
            //!为工厂创建流
            hr = m_pIWICFactory->CreateStream(&pStream);
        }
        if (SUCCEEDED(hr))
        {
            //!用指定的图片文件来初始化流
            hr = pStream->InitializeFromMemory(
                reinterpret_cast<BYTE*>(pImageFile),
                imageFileSize);
        }
        if (SUCCEEDED(hr))
        {
            //!为工厂创建解码器
            hr = m_pIWICFactory->CreateDecoderFromStream(
                pStream,
                nullptr,
                WICDecodeMetadataCacheOnLoad,
                &pDecoder);
        }
        if (SUCCEEDED(hr))
        {
            //!解码器获取帧
            hr = pDecoder->GetFrame(0, &pSource);
        }
        if (SUCCEEDED(hr))
        {
            // Convert the image format to 32bppPBGRA
            // (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
            //!为工厂创建格式转换者
            hr = m_pIWICFactory->CreateFormatConverter(&pConverter);
        }
        if (SUCCEEDED(hr))
        {
            //!初始化转换者
            hr = pConverter->Initialize(
                pSource,
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone,
                nullptr,
                0.f,
                WICBitmapPaletteTypeMedianCut);
        }
        if (SUCCEEDED(hr))
        {
            //!终于结束了！创建图片到渲染目标
            hr = pRenderTarget->CreateBitmapFromWicBitmap(
                pConverter,
                nullptr,
                &m_pBitmap); //!ppBitmap第一次出现
        }
        SafeReleaseInterface(&pDecoder);
        SafeReleaseInterface(&pSource);
        SafeReleaseInterface(&pStream);
        SafeReleaseInterface(&pConverter);
        return hr;
    }

    HRESULT Img::LoadBitmapFromFile(
        ID2D1RenderTarget*  pRenderTarget)
    {
        IWICBitmapDecoder*     pDecoder{ nullptr }; // 解码器
        IWICBitmapFrameDecode* pSource{ nullptr }; // 资源
        IWICFormatConverter*   pConverter{ nullptr }; // 转换者

        //!根据文件地址创建解码器
        HRESULT hr{ m_pIWICFactory->CreateDecoderFromFilename(
            m_uri,
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnLoad,
            &pDecoder) };

        if (SUCCEEDED(hr))
        {
            //!解码器获取帧
            hr = pDecoder->GetFrame(0, &pSource);
        }
        if (SUCCEEDED(hr))
        {
            //!为工厂创建格式转换者
            hr = m_pIWICFactory->CreateFormatConverter(&pConverter);
        }
        if (SUCCEEDED(hr))
        {
            //!初始化转换者
            hr = pConverter->Initialize(
                pSource,
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapDitherTypeNone,
                nullptr,
                0.f,
                WICBitmapPaletteTypeMedianCut);
        }
        if (SUCCEEDED(hr))
        {
            //!结束！创建图片到渲染目标
            hr = pRenderTarget->CreateBitmapFromWicBitmap(
                pConverter,
                nullptr,
                &m_pBitmap); //!ppBitmap第一次出现
        }
        SafeReleaseInterface(&pDecoder);
        SafeReleaseInterface(&pSource);
        SafeReleaseInterface(&pConverter);
        return hr;
    }
}
