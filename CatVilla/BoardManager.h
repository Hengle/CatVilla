#pragma once
#include "stdafx.h"
#include "TBoardManager.h"
#include "Character.h"
#include "Tile.h"
#include "Cursor.h"

namespace NumbFish
{
    class BoardManager // ���̹�����
    {
    public:
        void        StartManage             (ID2D1RenderTarget* pRenderTarget);                 // ��ʼ����
        void        EndManage               ();                                                 // ��������
        void        Draw                    ();                                                 // ��������
        void        SetCursor               (const Paths& paths, const BoardPos& pos);          // ���ù��
        void        SetBar                  (const Paths& paths, const BoardPos& pos);          // ���ý�����
        void        SetBarToCharacter       (Tag tag);                                          // ����ɫ�趨��������ʽ
        void        RemoveBarFromCharacter  (Tag tag);                                          // �Ƴ���ɫ��ӵ�еĽ�����
        void        AddTile                 (const PspTile& object);                            // ��Ӵ�ש
        void        AddTile                 (Tag tag, const Paths& paths, const BoardPos& p0);  // ��Ӵ�ש
        void        AddCharacter            (const PspCharacter& character);                    // ��ӽ�ɫ
        void        AddCharacter            (Tag tag, const Paths& paths, const BoardPos& p0);  // ��ӽ�ɫ
        bool        Delete                  (Tag tag);                                          // ɾ������
        bool        Place                   (const RowCol& pos, Tag tag);                       // ���ö���
        void        MoveCursor              (const RowCol& pos);                                // �ƶ����
        bool        Move                    (const RowCol& pos, Tag tag);                       // �ƶ�����
        bool        Remove                  (Tag tag);                                          // �Ƴ�����
        SpCursor    GetCursor               ();                                                 // ��ȡ���
        SpCharacter GetCharacter            (Tag tag);                                          // ��ȡ��ɫ
        SpTile      GetTile                 (Tag tag);                                          // ��ȡ��ש

    private:
        VspImg      Paths2Imgs              (const Paths& paths);                               // ��ȡ·�������е�����ͼƬ

        ID2D1RenderTarget*                  m_pRenderTarget;                                    // D2D��ȾĿ��
        TBoardManager<Character>            m_character;                                        // ��ɫ������
        TBoardManager<Tile>                 m_tile;                                             // ��ש������
        SpCursor                            m_cursor;                                           // ���
        SpBar                               m_bar;                                              // ������
    };
}
