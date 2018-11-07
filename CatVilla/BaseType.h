#pragma once
#include "stdafx.h"

namespace NumbFish
{
    typedef std::vector<std::wstring_view> Paths; // 路径集合

    /*todo ## 颜色枚举 ## */
    enum Color
    {
        Colorless = -1, // 无色

        AliceBlue = 0xF0F8FF,
        AntiqueWhite = 0xFAEBD7,
        Aqua = 0x00FFFF,
        Aquamarine = 0x7FFFD4,
        Azure = 0xF0FFFF,
        Beige = 0xF5F5DC,
        Bisque = 0xFFE4C4,
        Black = 0x000000,
        BlanchedAlmond = 0xFFEBCD,
        Blue = 0x0000FF,
        BlueViolet = 0x8A2BE2,
        Brown = 0xA52A2A,
        BurlyWood = 0xDEB887,
        CadetBlue = 0x5F9EA0,
        Chartreuse = 0x7FFF00,
        Chocolate = 0xD2691E,
        Coral = 0xFF7F50,
        CornflowerBlue = 0x6495ED,
        Cornsilk = 0xFFF8DC,
        Crimson = 0xDC143C,
        Cyan = 0x00FFFF,
        DarkBlue = 0x00008B,
        DarkCyan = 0x008B8B,
        DarkGoldenrod = 0xB8860B,
        DarkGray = 0xA9A9A9,
        DarkGreen = 0x006400,
        DarkKhaki = 0xBDB76B,
        DarkMagenta = 0x8B008B,
        DarkOliveGreen = 0x556B2F,
        DarkOrange = 0xFF8C00,
        DarkOrchid = 0x9932CC,
        DarkRed = 0x8B0000,
        DarkSalmon = 0xE9967A,
        DarkSeaGreen = 0x8FBC8F,
        DarkSlateBlue = 0x483D8B,
        DarkSlateGray = 0x2F4F4F,
        DarkTurquoise = 0x00CED1,
        DarkViolet = 0x9400D3,
        DeepPink = 0xFF1493,
        DeepSkyBlue = 0x00BFFF,
        DimGray = 0x696969,
        DodgerBlue = 0x1E90FF,
        Firebrick = 0xB22222,
        FloralWhite = 0xFFFAF0,
        ForestGreen = 0x228B22,
        Fuchsia = 0xFF00FF,
        Gainsboro = 0xDCDCDC,
        GhostWhite = 0xF8F8FF,
        Gold = 0xFFD700,
        Goldenrod = 0xDAA520,
        Gray = 0x808080,
        Green = 0x008000,
        GreenYellow = 0xADFF2F,
        Honeydew = 0xF0FFF0,
        HotPink = 0xFF69B4,
        IndianRed = 0xCD5C5C,
        Indigo = 0x4B0082,
        Ivory = 0xFFFFF0,
        Khaki = 0xF0E68C,
        Lavender = 0xE6E6FA,
        LavenderBlush = 0xFFF0F5,
        LawnGreen = 0x7CFC00,
        LemonChiffon = 0xFFFACD,
        LightBlue = 0xADD8E6,
        LightCoral = 0xF08080,
        LightCyan = 0xE0FFFF,
        LightGoldenrodYellow = 0xFAFAD2,
        LightGreen = 0x90EE90,
        LightGray = 0xD3D3D3,
        LightPink = 0xFFB6C1,
        LightSalmon = 0xFFA07A,
        LightSeaGreen = 0x20B2AA,
        LightSkyBlue = 0x87CEFA,
        LightSlateGray = 0x778899,
        LightSteelBlue = 0xB0C4DE,
        LightYellow = 0xFFFFE0,
        Lime = 0x00FF00,
        LimeGreen = 0x32CD32,
        Linen = 0xFAF0E6,
        Magenta = 0xFF00FF,
        Maroon = 0x800000,
        MediumAquamarine = 0x66CDAA,
        MediumBlue = 0x0000CD,
        MediumOrchid = 0xBA55D3,
        MediumPurple = 0x9370DB,
        MediumSeaGreen = 0x3CB371,
        MediumSlateBlue = 0x7B68EE,
        MediumSpringGreen = 0x00FA9A,
        MediumTurquoise = 0x48D1CC,
        MediumVioletRed = 0xC71585,
        MidnightBlue = 0x191970,
        MintCream = 0xF5FFFA,
        MistyRose = 0xFFE4E1,
        Moccasin = 0xFFE4B5,
        NavajoWhite = 0xFFDEAD,
        Navy = 0x000080,
        OldLace = 0xFDF5E6,
        Olive = 0x808000,
        OliveDrab = 0x6B8E23,
        Orange = 0xFFA500,
        OrangeRed = 0xFF4500,
        Orchid = 0xDA70D6,
        PaleGoldenrod = 0xEEE8AA,
        PaleGreen = 0x98FB98,
        PaleTurquoise = 0xAFEEEE,
        PaleVioletRed = 0xDB7093,
        PapayaWhip = 0xFFEFD5,
        PeachPuff = 0xFFDAB9,
        Peru = 0xCD853F,
        Pink = 0xFFC0CB,
        Plum = 0xDDA0DD,
        PowderBlue = 0xB0E0E6,
        Purple = 0x800080,
        Red = 0xFF0000,
        RosyBrown = 0xBC8F8F,
        RoyalBlue = 0x4169E1,
        SaddleBrown = 0x8B4513,
        Salmon = 0xFA8072,
        SandyBrown = 0xF4A460,
        SeaGreen = 0x2E8B57,
        SeaShell = 0xFFF5EE,
        Sienna = 0xA0522D,
        Silver = 0xC0C0C0,
        SkyBlue = 0x87CEEB,
        SlateBlue = 0x6A5ACD,
        SlateGray = 0x708090,
        Snow = 0xFFFAFA,
        SpringGreen = 0x00FF7F,
        SteelBlue = 0x4682B4,
        Tan = 0xD2B48C,
        Teal = 0x008080,
        Thistle = 0xD8BFD8,
        Tomato = 0xFF6347,
        Turquoise = 0x40E0D0,
        Violet = 0xEE82EE,
        Wheat = 0xF5DEB3,
        White = 0xFFFFFF,
        WhiteSmoke = 0xF5F5F5,
        Yellow = 0xFFFF00,
        YellowGreen = 0x9ACD32,
    };

    /*todo ## 点 ## */
    typedef class Point
    {
    public:
        int x = 0, y = 0;
        int& left = x;
        int& top = y;

        Point(int _x = 0, int _y = 0);

        Point& operator= (const Point&);
        bool   operator==(const Point&) const;
        bool   operator!=(const Point&) const;
        Point  operator+ (const Point&) const;
        Point  operator- (const Point&) const;
        Point& operator+=(const Point&);
        Point& operator-=(const Point&);
    }Step; /*todo ## 步长 ## */

    typedef std::vector<Point> Points;
    typedef Points Offset; // 每帧偏移量

    /*todo ## 默认步长 ## */
    const Step DEFAULT_STEP{ 46, 15 };

    /*todo ## 行列，一般情况下是maxRow = 3, maxCol = 8 ## */
    class RowCol
    {
    public:
        int row = 0, col = 0;
        RowCol(int r = 0, int c = 0);

        bool operator==(const RowCol&) const;
        bool operator!=(const RowCol&) const;
        bool operator< (const RowCol&) const;
        bool operator> (const RowCol&) const;
        bool operator<=(const RowCol&) const;
        bool operator>=(const RowCol&) const;
    };

    typedef std::set<RowCol> RowColSet; // 行列集合

    /*todo ## 宽高 ## */
    class WidthHeight
    {
    public:
        int width = 0, height = 0;
        WidthHeight(int w = 0, int h = 0);
    };

    /*todo ## 宽高百分比 ## */
    class WidthHeightPercent
    {
    public:
        FLOAT widthPer = 1.f, heightPer = 1.f;
        WidthHeightPercent(FLOAT wp = 1.f, FLOAT hp = 1.f);
    };

    /*todo ## 图像变换 ## */
    class Transform
    {
    public:
        bool rotate90 = false; // 是否旋转90度
        bool flipHor = false;  // 是否水平翻转
        bool flipVer = false;  // 是否垂直翻转
        Transform(bool r90 = false, bool fh = false, bool fv = false);
    };

    /*todo ## 棋盘坐标 ## */
    class BoardPos
    {
    public:
        BoardPos(const Point& originScreenPos = Point(), const Offset& offset = Offset(), const Step& step = DEFAULT_STEP);

        void   SetStep(const Step& step);
        Step   GetStep() const;
        void   SetOriginScreenPos(const Point& originScreenPos);
        Point  GetOriginScreenPos() const;
        Point  GetScreenPos() const;
        RowCol GetBoardPos() const;
        void   SetBoardPos(const RowCol&);
        void   SetOffset(const Offset&);
        Offset GetOffset() const;

    private:
        void CalPos(); // todo 根据boardPos计算screenPos

        Step   m_step = DEFAULT_STEP;
        Point  m_originScreenPos = Point(0, 0);
        Point  m_screenPos = Point(0, 0);
        RowCol m_boardPos = RowCol(0, 0);
        Offset m_offset; // 每帧坐标补偿
    };
}
