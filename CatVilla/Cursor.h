#pragma once
#include "stdafx.h"
#include "BoardObject.h"

namespace NumbFish
{
    class Cursor;
    class CursorState;

    typedef std::shared_ptr<Cursor>      SpCursor;
    typedef std::shared_ptr<CursorState> SpCursorState;

    enum CursorFloat {  //!光标浮动特效
        CF_NONE = -1,   // 无
        CF_HOR,         // 水平
        CF_VER          // 垂直
    };

    enum CursorStateEnum {  //!光标状态枚举
        CS_IDLE,
        CS_SELECT
    };

    class Cursor : public BoardObject //!光标
    {
    public:
        using BoardObject::BoardObject;
        void Draw(                                      //!绘制
            ID2D1RenderTarget*  pRenderTarget,          // 渲染目标
            const RowCol&       rc);                    // 坐标
        void DrawTileCursor(                            //!绘制瓷砖光标
            ID2D1RenderTarget*  pRenderTarget,          // 渲染目标
            const RowColSet&    vrSet);                 // 坐标集合 
        void SetFloat(CursorFloat cursorFloat);         //!设置光标浮动效果
        CursorFloat GetFloat() const;                   //!获取光标浮动效果
        void SetState(SpCursorState state);             //!设置光标状态
        bool Request(CursorStateEnum state);            //!请求更换状态
        void SetCursorPos(const RowCol& pos);           //!设置光标位置
        RowCol GetCursorPos() const;                    //!获取光标位置
        void SetPassList(const RowColSet& passList);    //!设置通行表
        RowColSet GetPassList() const;                  //!获取通行表

    private:
        CursorFloat     m_float = CF_NONE;              // 光标浮动效果，默认无
        SpCursorState   m_state;                        // 光标状态
        RowCol          m_cursorPos;                    // 光标位置
        RowColSet       m_passList;                     // 通行表
    };

    class CursorState //!光标状态
    {
    public:
        CursorState(SpCursor cursor);
        virtual ~CursorState() {};
        virtual bool Handle(CursorStateEnum state) = 0;

    protected:
        SpCursor m_cursor;
    };

    class CursorIdleState : public CursorState //!光标空闲状态
    {
    public:
        using CursorState::CursorState;
        bool Handle(CursorStateEnum state);
    };

    class CursorSelectState : public CursorState //!光标选择状态
    {
    public:
        using CursorState::CursorState;
        bool Handle(CursorStateEnum state);
    };
}
