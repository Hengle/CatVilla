#pragma once
#include "stdafx.h"

namespace NumbFish
{
    /*todo ## ͼƬ ## */
    class Img
    {
    public:
        Img() = delete; // ��ֹĬ��
        Img(ID2D1RenderTarget* pRenderTarget, const PCWSTR& uri); // ����
        Img(const Img&); // ����
        Img(Img&&); // �ƹ�
        ~Img(); // ����

        Img& operator=(const Img&); // ��ֵ
        Img& operator=(Img&&); // �Ƹ�

        HRESULT Create(); // ����
        HRESULT Draw( // ����
            ID2D1RenderTarget*        pRenderTarget, // ��ȾĿ��
            const Point&              point,         // ����λ��
            FLOAT                     opacity = 1.f, // ͸����
            const Transform&          transform = Transform(),       // ͼ��ת��
            const WidthHeightPercent& WHPer = WidthHeightPercent()); // ͼ��ٷֱȿ���
        HRESULT DrawClip( // ���Ʋ���ͼ��
            ID2D1RenderTarget*        pRenderTarget, // ��ȾĿ��
            const Point&              point,         // ����λ��
            const Point&              pointLeftTop,  // ��ԭͼָ��λ�ÿ�ʼ����
            const WidthHeight&        WH,            // ����ͼ��Ŀ��
            FLOAT                     opacity = 1.f, // ͸����
            const Transform&          transform = Transform(),       // ͼ��ת��
            const WidthHeightPercent& WHPer = WidthHeightPercent()); // ͼ��ٷֱȿ���
        D2D1_SIZE_F GetSize() const; // ��ȡͼƬ�ߴ�

    public:
        void Release(); // �ͷ�

    private:
        void swap(Img&) noexcept; // ����

    private:
        HRESULT LoadBitmapFromResource( // ��ȡ����Դ
            ID2D1RenderTarget* pRenderTarget,
            const PCWSTR&      resourceName,
            const PCWSTR&      resourceType);

        HRESULT LoadBitmapFromFile(ID2D1RenderTarget* pRenderTarget); // ��ȡ���ļ�

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
