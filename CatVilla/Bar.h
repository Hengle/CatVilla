#pragma once
#include "stdafx.h"
#include "BoardObject.h"

namespace NumbFish
{
    class BarInfo // ��������Ϣ
    {
    public:
        BarInfo(                        //!����
            int     percent = 0,        // �ٷֱȣ�Ĭ��0
            int     speed   = 10,       // �ٶȣ�Ĭ��10
            bool    start   = true);    // ��ͣ��Ĭ����
        int  m_percent;                 // �ٷֱ�
        int  m_speed;                   // �ٶ�
        bool m_start;                   // ��ͣ
    };

    class Bar : public BoardObject  //!������
    {
    public:
        using BoardObject::BoardObject;
        void Draw(                              //!���ƽ�������
            ID2D1RenderTarget*  pRenderTarget,  // ��ȾĿ��
            const RowCol&       rc);            // ����
        void DrawProcess(                       //!���ƽ���������
            ID2D1RenderTarget*  pRenderTarget,  // ��ȾĿ��
            const RowCol&       rc,             // ����
            const BarInfo&      barInfo);       // ��������Ϣ
    };

    typedef std::shared_ptr<Bar> SpBar;
}
