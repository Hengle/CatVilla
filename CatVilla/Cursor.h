#pragma once
#include "stdafx.h"
#include "BoardObject.h"

namespace NumbFish
{
    class Cursor;
    class CursorState;

    typedef std::shared_ptr<Cursor>      SpCursor;
    typedef std::shared_ptr<CursorState> SpCursorState;

    enum CursorFloat {  //!��긡����Ч
        CF_NONE = -1,   // ��
        CF_HOR,         // ˮƽ
        CF_VER          // ��ֱ
    };

    enum CursorStateEnum {  //!���״̬ö��
        CS_IDLE,
        CS_SELECT
    };

    class Cursor : public BoardObject //!���
    {
    public:
        using BoardObject::BoardObject;
        void Draw(                                      //!����
            ID2D1RenderTarget*  pRenderTarget,          // ��ȾĿ��
            const RowCol&       rc);                    // ����
        void DrawTileCursor(                            //!���ƴ�ש���
            ID2D1RenderTarget*  pRenderTarget,          // ��ȾĿ��
            const RowColSet&    vrSet);                 // ���꼯�� 
        void SetFloat(CursorFloat cursorFloat);         //!���ù�긡��Ч��
        CursorFloat GetFloat() const;                   //!��ȡ��긡��Ч��
        void SetState(SpCursorState state);             //!���ù��״̬
        bool Request(CursorStateEnum state);            //!�������״̬
        void SetCursorPos(const RowCol& pos);           //!���ù��λ��
        RowCol GetCursorPos() const;                    //!��ȡ���λ��
        void SetPassList(const RowColSet& passList);    //!����ͨ�б�
        RowColSet GetPassList() const;                  //!��ȡͨ�б�

    private:
        CursorFloat     m_float = CF_NONE;              // ��긡��Ч����Ĭ����
        SpCursorState   m_state;                        // ���״̬
        RowCol          m_cursorPos;                    // ���λ��
        RowColSet       m_passList;                     // ͨ�б�
    };

    class CursorState //!���״̬
    {
    public:
        CursorState(SpCursor cursor);
        virtual ~CursorState() {};
        virtual bool Handle(CursorStateEnum state) = 0;

    protected:
        SpCursor m_cursor;
    };

    class CursorIdleState : public CursorState //!������״̬
    {
    public:
        using CursorState::CursorState;
        bool Handle(CursorStateEnum state);
    };

    class CursorSelectState : public CursorState //!���ѡ��״̬
    {
    public:
        using CursorState::CursorState;
        bool Handle(CursorStateEnum state);
    };
}
