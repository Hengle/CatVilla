#pragma once
#include "stdafx.h"

namespace NumbFish
{
    typedef std::string_view Tag; // 标签

    template<class T>
    class TBoardManager // 棋盘管理者模板
    {
    public:
        typedef std::shared_ptr<T>           SpT;
        typedef std::pair<std::string, SpT> PspT;
        typedef std::map<std::string, SpT>  MspT;

        void    Draw    (ID2D1RenderTarget* pRenderTarget); // 绘制
        void    Add     (const PspT& object);               // 添加
        bool    Delete  (Tag tag);                          // 删除
        bool    Place   (const RowCol& pos, Tag tag);       // 放置
        bool    Remove  (Tag tag);                          // 移除
        bool    Move    (const RowCol& pos, Tag tag);       // 移动
        SpT     Find    (Tag tag);                          // 寻找

    private:
        PspT m_showBoard[3][8]; // 展示板
        MspT m_showList;        // 展示列表
        MspT m_warehouse;       // 仓库
    };

    // 用于模板的特殊性，需要在头文件中定义
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
