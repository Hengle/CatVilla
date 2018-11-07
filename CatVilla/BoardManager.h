#pragma once
#include "stdafx.h"
#include "TBoardManager.h"
#include "Character.h"
#include "Tile.h"
#include "Cursor.h"

namespace NumbFish
{
    class BoardManager // 棋盘管理者
    {
    public:
        void        StartManage             (ID2D1RenderTarget* pRenderTarget);                 // 开始管理
        void        EndManage               ();                                                 // 结束管理
        void        Draw                    ();                                                 // 绘制棋盘
        void        SetCursor               (const Paths& paths, const BoardPos& pos);          // 设置光标
        void        SetBar                  (const Paths& paths, const BoardPos& pos);          // 设置进度条
        void        SetBarToCharacter       (Tag tag);                                          // 给角色设定进度条样式
        void        RemoveBarFromCharacter  (Tag tag);                                          // 移除角色所拥有的进度条
        void        AddTile                 (const PspTile& object);                            // 添加瓷砖
        void        AddTile                 (Tag tag, const Paths& paths, const BoardPos& p0);  // 添加瓷砖
        void        AddCharacter            (const PspCharacter& character);                    // 添加角色
        void        AddCharacter            (Tag tag, const Paths& paths, const BoardPos& p0);  // 添加角色
        bool        Delete                  (Tag tag);                                          // 删除对象
        bool        Place                   (const RowCol& pos, Tag tag);                       // 放置对象
        void        MoveCursor              (const RowCol& pos);                                // 移动光标
        bool        Move                    (const RowCol& pos, Tag tag);                       // 移动对象
        bool        Remove                  (Tag tag);                                          // 移除对象
        SpCursor    GetCursor               ();                                                 // 获取光标
        SpCharacter GetCharacter            (Tag tag);                                          // 获取角色
        SpTile      GetTile                 (Tag tag);                                          // 获取瓷砖

    private:
        VspImg      Paths2Imgs              (const Paths& paths);                               // 获取路径集合中的所有图片

        ID2D1RenderTarget*                  m_pRenderTarget;                                    // D2D渲染目标
        TBoardManager<Character>            m_character;                                        // 角色管理者
        TBoardManager<Tile>                 m_tile;                                             // 瓷砖管理者
        SpCursor                            m_cursor;                                           // 光标
        SpBar                               m_bar;                                              // 进度条
    };
}
