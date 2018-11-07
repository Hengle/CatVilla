#include "stdafx.h"
#include "BaseType.h"

namespace NumbFish
{
           Point::Point     (int _x, int _y)       : x(_x), y(_y) { }
    Point& Point::operator= (const Point& p)       { x = p.x; y = p.y; return *this; }
    bool   Point::operator==(const Point& p) const { return (x == p.x && y == p.y); }
    bool   Point::operator!=(const Point& p) const { return !(*this == p); }
    Point  Point::operator+ (const Point& p) const { return Point(this->x + p.x, this->y + p.y); }
    Point  Point::operator- (const Point& p) const { return Point(this->x - p.x, this->y - p.y); }
    Point& Point::operator+=(const Point& p)       { this->x += p.x; this->y += p.y; return *this; }
    Point& Point::operator-=(const Point& p)       { this->x -= p.x; this->y -= p.y; return *this; }

            RowCol::RowCol    (int r, int c)           : row(r),      col(c)      { }
    bool    RowCol::operator==(const RowCol& rc) const { return (row == rc.row && col == rc.col);}
    bool    RowCol::operator!=(const RowCol& rc) const { return !(*this == rc); }
    bool    RowCol::operator< (const RowCol& rc) const { return (row < rc.row || (row == rc.row && col < rc.col)); }
    bool    RowCol::operator> (const RowCol& rc) const { return (row > rc.row || (row == rc.row && col > rc.col)); }
    bool    RowCol::operator<=(const RowCol& rc) const { return !(*this > rc); }
    bool    RowCol::operator>=(const RowCol& rc) const { return !(*this < rc); }

    WidthHeight::WidthHeight(int w, int h) : width(w), height(h) { }

    WidthHeightPercent::WidthHeightPercent(FLOAT wp, FLOAT hp) : widthPer(wp),  heightPer(hp)  { }

    Transform::Transform(bool r90, bool fh, bool fv) : rotate90(r90), flipHor(fh), flipVer(fv) { }

           BoardPos::BoardPos          (const Point& p0, const Offset& offset, const Step& step) : m_offset(offset)
                                                          { SetOriginScreenPos(p0); SetStep(step); }
    void   BoardPos::SetStep           (const Step& step) { m_step = step; }
    Step   BoardPos::GetStep           () const           { return m_step; }
    void   BoardPos::SetOriginScreenPos(const Point& p0)  { m_originScreenPos = p0; CalPos(); }
    Point  BoardPos::GetOriginScreenPos() const           { return m_originScreenPos; }
    Point  BoardPos::GetScreenPos      () const           { return m_screenPos; }
    RowCol BoardPos::GetBoardPos       () const           { return m_boardPos; }
    void   BoardPos::SetBoardPos       (const RowCol& rc) { m_boardPos = rc; CalPos(); }
    void   BoardPos::SetOffset         (const Offset& os) { m_offset = os; }
    Offset BoardPos::GetOffset         () const           { return m_offset; }
    void   BoardPos::CalPos            ()
    {
        m_screenPos.x
            = m_originScreenPos.x
            + m_boardPos.col * m_step.x
            - m_boardPos.row * m_step.y;
        m_screenPos.y
            = m_originScreenPos.y
            + m_boardPos.row * m_step.y;
    }
}
