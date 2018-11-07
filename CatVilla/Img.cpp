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
            //!��ʼ��IWIC����
            hr = CoCreateInstance(
                CLSID_WICImagingFactory,
                nullptr,
                CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(&m_pIWICFactory));
        }
        if (SUCCEEDED(hr))
        {
            //!��ȡͼƬ��Դ
            hr = LoadBitmapFromFile(m_pRenderTarget);
        }
        if (FAILED(hr))
        {
            Release();
        }
        return hr;
    }

    HRESULT Img::Draw( // ����
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
        IWICBitmapDecoder*     pDecoder{ nullptr }; // ����
        IWICBitmapFrameDecode* pSource{ nullptr }; // ��Դ
        IWICStream*            pStream{ nullptr }; // ��
        IWICFormatConverter*   pConverter{ nullptr }; // ��ʽת��

        HRSRC   imageResHandle{ nullptr }; // ͼƬ��Դ���
        HGLOBAL imageResDataHandle{ nullptr }; // ͼƬ��Դ���ݾ��
        void*   pImageFile{ nullptr }; // ͼƬ�ļ�
        DWORD   imageFileSize{ 0 };    // ͼƬ��С

        // ������Դ���ƺ�����Ѱ����Դ
        //!�˴����ͼƬ��Դ���
        imageResHandle = FindResourceW(HINST_THISCOMPONENT, resourceName, resourceType);
        HRESULT hr{ imageResHandle ? S_OK : E_FAIL };
        // ����Ŀ�ʼ
        if (SUCCEEDED(hr))
        {
            // ����ͼƬ��Դ������ͼƬ���ݾ��
            //!�˴����ͼƬ��Դ���ݾ��
            imageResDataHandle = LoadResource(HINST_THISCOMPONENT, imageResHandle);
            hr = imageResDataHandle ? S_OK : E_FAIL;
        }
        if (SUCCEEDED(hr))
        {
            // �����ڴ��е�ָ����Դ����������Դ���ڴ��еĵ�ַ�������������
            //!�˴����ͼƬ�ļ�
            pImageFile = LockResource(imageResDataHandle);
            hr = pImageFile ? S_OK : E_FAIL;
        }
        if (SUCCEEDED(hr))
        {
            //!�˴����ͼƬ�ļ���С
            imageFileSize = SizeofResource(HINST_THISCOMPONENT, imageResHandle);
            hr = imageFileSize ? S_OK : E_FAIL;
        }
        if (SUCCEEDED(hr))
        {
            //!Ϊ����������
            hr = m_pIWICFactory->CreateStream(&pStream);
        }
        if (SUCCEEDED(hr))
        {
            //!��ָ����ͼƬ�ļ�����ʼ����
            hr = pStream->InitializeFromMemory(
                reinterpret_cast<BYTE*>(pImageFile),
                imageFileSize);
        }
        if (SUCCEEDED(hr))
        {
            //!Ϊ��������������
            hr = m_pIWICFactory->CreateDecoderFromStream(
                pStream,
                nullptr,
                WICDecodeMetadataCacheOnLoad,
                &pDecoder);
        }
        if (SUCCEEDED(hr))
        {
            //!��������ȡ֡
            hr = pDecoder->GetFrame(0, &pSource);
        }
        if (SUCCEEDED(hr))
        {
            // Convert the image format to 32bppPBGRA
            // (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
            //!Ϊ����������ʽת����
            hr = m_pIWICFactory->CreateFormatConverter(&pConverter);
        }
        if (SUCCEEDED(hr))
        {
            //!��ʼ��ת����
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
            //!���ڽ����ˣ�����ͼƬ����ȾĿ��
            hr = pRenderTarget->CreateBitmapFromWicBitmap(
                pConverter,
                nullptr,
                &m_pBitmap); //!ppBitmap��һ�γ���
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
        IWICBitmapDecoder*     pDecoder{ nullptr }; // ������
        IWICBitmapFrameDecode* pSource{ nullptr }; // ��Դ
        IWICFormatConverter*   pConverter{ nullptr }; // ת����

        //!�����ļ���ַ����������
        HRESULT hr{ m_pIWICFactory->CreateDecoderFromFilename(
            m_uri,
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnLoad,
            &pDecoder) };

        if (SUCCEEDED(hr))
        {
            //!��������ȡ֡
            hr = pDecoder->GetFrame(0, &pSource);
        }
        if (SUCCEEDED(hr))
        {
            //!Ϊ����������ʽת����
            hr = m_pIWICFactory->CreateFormatConverter(&pConverter);
        }
        if (SUCCEEDED(hr))
        {
            //!��ʼ��ת����
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
            //!����������ͼƬ����ȾĿ��
            hr = pRenderTarget->CreateBitmapFromWicBitmap(
                pConverter,
                nullptr,
                &m_pBitmap); //!ppBitmap��һ�γ���
        }
        SafeReleaseInterface(&pDecoder);
        SafeReleaseInterface(&pSource);
        SafeReleaseInterface(&pConverter);
        return hr;
    }
}
