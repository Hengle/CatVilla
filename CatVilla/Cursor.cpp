#include "stdafx.h"
#include "Cursor.h"

namespace NumbFish
{
    void Cursor::Draw(ID2D1RenderTarget* pRenderTarget, const RowCol& rc)
    {
        if (m_imgs.size() <= 0 || m_imgs[0] == nullptr) { return; }

        m_pos.SetBoardPos(rc);

        const int SPEED = 100;
        const int FRAME = 10;
        const int cursorFloat[FRAME]{ -2, -1, 0, 1, 2, 2, 1, 0, -1, -2 };
        static auto time = GetTickCount();
        auto i = ((GetTickCount() - time) / SPEED) % FRAME;
        if (GetTickCount() - time > SPEED * FRAME)
        {
            time = GetTickCount();
        }

        auto pos = m_pos.GetScreenPos();
        switch (m_float)
        {
        case CF_HOR:
            pos += Point(cursorFloat[i], 0);
            break;
        case CF_VER:
            pos += Point(0, cursorFloat[i]);
            break;
        default:
            break;
        }
        m_imgs[0]->Draw(pRenderTarget, pos);
    }
    void Cursor::DrawTileCursor(ID2D1RenderTarget* pRenderTarget, const RowColSet& rcSet)
    {
        if (m_imgs.size() <= 1 || m_imgs[1] == nullptr) { return; }

        const int SPEED = 50;
        const int FRAME = 24;
        static auto time = GetTickCount();
        auto i = ((GetTickCount() - time) / SPEED) % FRAME;
        if (GetTickCount() - time > SPEED * FRAME)
        {
            time = GetTickCount();
        }
        auto opacity = (sin(6.28f / FRAME * i) + 1) / 2.0f;

        auto pos = m_pos;
        for (const auto& rc : rcSet)
        {
            pos.SetBoardPos(rc);
            Point offset{ 0, 0 };
            if (m_pos.GetOffset().size() >= 1)
            {
                offset = m_pos.GetOffset()[0];
            }
            m_imgs[1]->Draw(pRenderTarget, pos.GetScreenPos() + offset, opacity);
        }
    }
    void Cursor::SetFloat(CursorFloat cursorFloat)
    {
        m_float = cursorFloat;
    }
    CursorFloat Cursor::GetFloat() const
    {
        return m_float;
    }
    void Cursor::SetState(SpCursorState state)
    {
        m_state = state;
    }
    bool Cursor::Request(CursorStateEnum state)
    {
        return m_state->Handle(state);
    }
    void Cursor::SetCursorPos(const RowCol& pos)
    {
        m_cursorPos = pos;
    }
    RowCol Cursor::GetCursorPos() const
    {
        return m_cursorPos;
    }
    void Cursor::SetPassList(const RowColSet& passList)
    {
        m_passList = passList;
    }
    RowColSet Cursor::GetPassList() const
    {
        return m_passList;
    }
    CursorState::CursorState(SpCursor cursor) : m_cursor(cursor) { }
    bool CursorIdleState::Handle(CursorStateEnum state)
    {
        switch(state)
        {
        case CS_SELECT:
            m_cursor->SetState(std::make_shared<CursorSelectState>(m_cursor));
            return true;
        default:
            return false;
        }
    }
    bool CursorSelectState::Handle(CursorStateEnum state)
    {
        switch (state)
        {
        case CS_IDLE:
            m_cursor->SetState(std::make_shared<CursorIdleState>(m_cursor));
            return true;
        default:
            return false;
        }
    }
}
