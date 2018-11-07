#pragma once
#include "stdafx.h"
#include "Bar.h"

namespace NumbFish
{
    class Character : public BoardObject //!��ɫ
    {
    public:
        using BoardObject::BoardObject;
        void Draw(                                  //!����
            ID2D1RenderTarget*  pRenderTarget,      // ��ȾĿ��
            const RowCol&       rc);                // ����
        void SetBar(SpBar bar);                     //!���ý�������ʽ
        void RemoveBar();                           //!�Ƴ�������
        void SetBarInfo(const BarInfo& barInfo);    //!���ý�������Ϣ
        BarInfo GetBarInfo() const;                 //!��ȡ��������Ϣ

    private:
        SpBar   m_bar;      // ������
        BarInfo m_barInfo;  // ��������Ϣ
    };

    typedef std::shared_ptr<Character>           SpCharacter;
    typedef std::pair<std::string, SpCharacter> PspCharacter;
}
