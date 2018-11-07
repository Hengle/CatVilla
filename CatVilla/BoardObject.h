#pragma once
#include "stdafx.h"
#include "Img.h"

namespace NumbFish
{
    class BoardObject //!���̶���������
    {
    public:
        BoardObject()                               = delete;   //!�ܾ�Ĭ��
        BoardObject(const BoardObject&)             = default;  //!Ĭ�ϸ���
        BoardObject(BoardObject&&)                  = default;  //!Ĭ���ƹ�
        ~BoardObject()                              = default;  //!Ĭ������
        BoardObject& operator=(const BoardObject&)  = default;  //!Ĭ�ϸ�ֵ
        BoardObject& operator=(BoardObject&&)       = default;  //!Ĭ���Ƹ�
        BoardObject(                                            //!����
            const VspImg&   imgs,                               // ͼ�񼯺�
            const BoardPos& pos);                               // ԭʼ����
        void virtual    Draw(                                   //!���ƽӿ�
            ID2D1RenderTarget*  pRenderTarget,                  // ��ȾĿ��
            const RowCol&       rc) = 0;                        // ����
        void            SetPos  (const BoardPos& pos);          //!��������
        BoardPos        GetPos  () const;                       //!��ȡ����

    protected:
        VspImg      m_imgs;                                     // ͼ�񼯺�
        BoardPos    m_pos;                                      // ����
    };
}
