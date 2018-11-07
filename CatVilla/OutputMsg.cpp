#include "stdafx.h"
#include "OutputMsg.h"

void OutputMsg(const wchar_t* msg)
{
    OutputDebugString(L"/*** ");
    OutputDebugString(msg);
    OutputDebugString(L" ***/\n");
}

void OutputMsg(const char* msg)
{
    OutputMsg(Str2Wstr(msg));
}

void OutputMsg(std::wstring_view msg)
{
    OutputMsg(msg.data());
}

void OutputMsg(std::string_view msg)
{
    OutputMsg(Str2Wstr(msg).c_str());
}

void OutputMsg(int msg)
{
    OutputMsg(std::to_wstring(msg).c_str());
}

void OutputMsg(const NumbFish::Point& p)
{
    std::wstring wstr = L"(x = " + std::to_wstring(p.x) + L", y = " + std::to_wstring(p.y) + L")";
    OutputMsg(wstr.c_str());
}

void OutputMsg(const NumbFish::RowCol& rc)
{
    std::wstring wstr = L"(row = " + std::to_wstring(rc.row) + L", col = " + std::to_wstring(rc.col) + L")";
    OutputMsg(wstr.c_str());
}

std::string Wstr2Str(std::wstring_view wstr)
{
    size_t i;
    std::string curLocale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "chs");
    const wchar_t* _source = wstr.data();
    size_t _dsize = 2 * wstr.size() + 1;
    char* _dest = new char[_dsize];
    memset(_dest, 0x0, _dsize);
    wcstombs_s(&i, _dest, _dsize, _source, _dsize);
    std::string result = _dest;
    delete[] _dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}

std::wstring Str2Wstr(std::string_view str)
{
    size_t i;
    std::string curLocale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "chs");
    const char* _source = str.data();
    size_t _dsize = str.size() + 1;
    wchar_t* _dest = new wchar_t[_dsize];
    wmemset(_dest, 0x0, _dsize);
    mbstowcs_s(&i, _dest, _dsize, _source, _dsize);
    std::wstring result = _dest;
    delete[] _dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}
