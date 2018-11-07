#pragma once
#include "stdafx.h"

namespace NumbFish
{
    typedef std::string_view Tag; // ��ǩ

    template<class T>
    class TBoardManager // ���̹�����ģ��
    {
    public:
        typedef std::shared_ptr<T>           SpT;
        typedef std::pair<std::string, SpT> PspT;
        typedef std::map<std::string, SpT>  MspT;

        void    Draw    (ID2D1RenderTarget* pRenderTarget); // ����
        void    Add     (const PspT& object);               // ���
        bool    Delete  (Tag tag);                          // ɾ��
        bool    Place   (const RowCol& pos, Tag tag);       // ����
        bool    Remove  (Tag tag);                          // �Ƴ�
        bool    Move    (const RowCol& pos, Tag tag);       // �ƶ�
        SpT     Find    (Tag tag);                          // Ѱ��

    private:
        PspT m_showBoard[3][8]; // չʾ��
        MspT m_showList;        // չʾ�б�
        MspT m_warehouse;       // �ֿ�
    };

    // ����ģ��������ԣ���Ҫ��ͷ�ļ��ж���
    template<class T> void TBoardManager<T>::Draw(ID2D1RenderTarget* pRenderTarget)
    {
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if (auto object = m_showBoard[i][j].second; object != nullptr)
                {
                    object->Draw(pRenderTarget, RowCol(i, j));
                }
            }
        }
    }
    template<class T> void TBoardManager<T>::Add(const PspT& object)
    {
        m_warehouse.insert(object);
    }
    template<class T> bool TBoardManager<T>::Delete(Tag tag)
    {
        Remove(tag);
        return (m_warehouse.erase(tag.data()) > 0);
    }
    template<class T> bool TBoardManager<T>::Place(const RowCol& pos, Tag tag)
    {
        auto[row, col] = pos;
        auto object = m_warehouse[tag.data()];
        if (object == nullptr) { return false; }
        m_showBoard[row][col] = std::pair<std::string, SpT>(tag, object);
        return true;
    }
    template<class T> bool TBoardManager<T>::Remove(Tag tag)
    {
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if (m_showBoard[i][j].first == tag)
                {
                    m_showBoard[i][j] = PspT();
                    m_showList.erase(tag.data());
                    return true;
                }
            }
        }
        return false;
    }
    template<class T> bool TBoardManager<T>::Move(const RowCol& pos, Tag tag)
    {
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if (m_showBoard[i][j].first == tag)
                {
                    PspT object{};
                    using std::swap;
                    swap(m_showBoard[i][j], object);
                    auto[row, col] = pos;
                    m_showBoard[row][col] = object;
                    return true;
                }
            }
        }
        return false;
    }
    template<class T> std::shared_ptr<T> TBoardManager<T>::Find(Tag tag)
    {
        return m_warehouse[tag.data()];
    }
}
