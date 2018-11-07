#pragma once
#include "stdafx.h"
#include "Bar.h"

namespace NumbFish
{
    class Character : public BoardObject //!角色
    {
    public:
        using BoardObject::BoardObject;
        void Draw(                                  //!绘制
            ID2D1RenderTarget*  pRenderTarget,      // 渲染目标
            const RowCol&       rc);                // 坐标
        void SetBar(SpBar bar);                     //!设置进度条样式
        void RemoveBar();                           //!移除进度条
        void SetBarInfo(const BarInfo& barInfo);    //!设置进度条信息
        BarInfo GetBarInfo() const;                 //!获取进度条信息

    private:
        SpBar   m_bar;      // 进度条
        BarInfo m_barInfo;  // 进度条信息
    };

    typedef std::shared_ptr<Character>           SpCharacter;
    typedef std::pair<std::string, SpCharacter> PspCharacter;
}
