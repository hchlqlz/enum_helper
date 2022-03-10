#pragma once

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
using namespace std;

vector<string> split(const string& str, const string& sep);

template<typename T>
string EnumHelper(T key, const std::function<char(char)> processor = nullptr, const char* pszName = NULL)
{
    static_assert(std::is_enum_v<T>, __FUNCTION__ "'s key need a enum");

    static map<T, string> s_mapName;

    if (nullptr != pszName)
    {
        s_mapName[key] = pszName;
    }

    std::string res = "";
    auto it = s_mapName.find(key);
    if (it != s_mapName.end())
    {
        res = it->second;
    }

    if (nullptr != processor)
    {
        std::transform(res.begin(), res.end(), res.begin(), processor);
    }

    return res;
}

template <class T>
size_t analystEnum(T enumClass, const char* pszNames)
{
    static_assert(std::is_enum_v<T>, __FUNCTION__ "'s enumClass need a enum");

    if (nullptr != pszNames)
    {
        const vector<string>& vecName = split(pszNames, ",");

        for (int i = 0; i < vecName.size(); ++i)
        {
            if (vecName.at(i).size() > 0)
            {
                EnumHelper((T)(i + 1), nullptr, vecName.at(i).c_str() + (i == 0 ? 0 : 1) );
            }
        }

        return vecName.size();
    }

    return 0;
}

#define ENUM_DEFINE(type, ...) enum class type { placeholder, __VA_ARGS__ }; const size_t g_uEnumSizeOf##type = analystEnum(type::placeholder, #__VA_ARGS__);

/*
ENUM_DEFINE 用法示例：
ENUM_DEFINE ( Color,
    Red,
    Blue,
)

EnumHelper(Color::Red) -> "Red"
EnumHelper(Color::Red, std::toupper) -> "RED"

注意点：
1、枚举值只能系统自增，不能写 Red = 1，代码未处理该种情况
*/