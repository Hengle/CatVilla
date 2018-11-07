#include "stdafx.h"
#include "BoardManager.h"

namespace NumbFish
{
    VspImg BoardManager::Paths2Imgs(const Paths& paths)
    {
        VspImg imgs;
        for (const auto& p : paths)
        {
            imgs.push_back(SpImg(std::make_shared<Img>(m_pRenderTarget, p.data())));
        }
        return imgs;
    }
    void BoardManager::StartManage(ID2D1RenderTarget* pRenderTarget)
    {
        m_pRenderTarget = pRenderTarget;
    }
    void BoardManager::EndManage()
    {
        m_pRenderTarget = nullptr;
    }
    void BoardManager::Draw()
    {
        auto tileCursorList = RowColSet();
        auto cursorPos = RowCol();
        if (m_cursor)
        {
            tileCursorList = m_cursor->GetPassList();
            cursorPos = m_cursor->GetCursorPos();
            tileCursorList.emplace(cursorPos);
        }

        m_tile.Draw(m_pRenderTarget);
        if (m_cursor) { m_cursor->DrawTileCursor(m_pRenderTarget, tileCursorList); }
        m_character.Draw(m_pRenderTarget);
        if (m_cursor) { m_cursor->Draw(m_pRenderTarget, cursorPos); }
    }
    void BoardManager::SetCursor(const Paths& paths, const BoardPos& pos)
    {
        m_cursor = std::make_shared<Cursor>(Paths2Imgs(paths), pos);
    }
    void BoardManager::SetBar(const Paths& paths, const BoardPos& pos)
    {
        m_bar = std::make_shared<Bar>(Paths2Imgs(paths), pos);
    }
    void BoardManager::SetBarToCharacter(Tag tag)
    {
        if (auto character = m_character.Find(tag); character != nullptr && m_bar != nullptr)
        {
            character->SetBar(m_bar);
        }
    }
    void BoardManager::RemoveBarFromCharacter(Tag tag)
    {
        if (auto character = m_character.Find(tag); character != nullptr && m_bar != nullptr)
        {
            character->RemoveBar();
        }
    }
    void BoardManager::AddTile(const PspTile& object)
    {
        m_tile.Add(object);
    }
    void BoardManager::AddTile(Tag tag, const Paths& paths, const BoardPos& p0)
    {
        AddTile(PspTile(tag, std::make_shared<Tile>(Paths2Imgs(paths), BoardPos(p0))));
    }
    void BoardManager::AddCharacter(const PspCharacter& object)
    {
        m_character.Add(object);
    }
    void BoardManager::AddCharacter(Tag tag, const Paths& paths, const BoardPos& p0)
    {
        AddCharacter(PspCharacter(tag, std::make_shared<Character>(Paths2Imgs(paths), BoardPos(p0))));
    }
    bool BoardManager::Delete(Tag tag)
    {
        if (m_tile.Delete(tag)) { return true; }
        if (m_character.Delete(tag)) { return true; }
        return false;
    }
    bool BoardManager::Place(const RowCol& pos, Tag tag)
    {
        if (m_tile.Place(pos, tag)) { return true; }
        if (m_character.Place(pos, tag)) { return true; }
        return false;
    }
    void BoardManager::MoveCursor(const RowCol& pos)
    {
        m_cursor->SetCursorPos(pos);
    }
    bool BoardManager::Move(const RowCol& pos, Tag tag)
    {
        if (m_tile.Move(pos, tag)) { return true; }
        if (m_character.Move(pos, tag)) { return true; }
        return false;
    }
    bool BoardManager::Remove(Tag tag)
    {
        if (m_tile.Remove(tag)) { return true; }
        if (m_character.Remove(tag)) { return true; }
        return false;
    }
    SpCursor BoardManager::GetCursor()
    {
        return m_cursor;
    }
    SpCharacter BoardManager::GetCharacter(Tag tag)
    {
        return m_character.Find(tag);
    }
    SpTile BoardManager::GetTile(Tag tag)
    {
        return m_tile.Find(tag);
    }
}
