#pragma once
#include "stdafx.h"

// ���������Ϣ
void OutputMsg(const wchar_t* msg);
void OutputMsg(const char*    msg);
void OutputMsg(std::wstring_view msg);
void OutputMsg(std::string_view msg);
void OutputMsg(int   msg);
void OutputMsg(const NumbFish::Point&  p);
void OutputMsg(const NumbFish::RowCol& rc);
std::string  Wstr2Str(std::wstring_view wstr); // wstring ת��Ϊ string
std::wstring Str2Wstr(std::string_view  str);  // string  ת��Ϊ wstring
