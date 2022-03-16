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
size_t ParseEnum(T enumClass, const char* pszNames)
{
    static_assert(std::is_enum_v<T>, __FUNCTION__ "'s enumClass need a enum");

    static size_t s_sizeOfEnum = 0;

    // 避免重复调用的开销
    if (s_sizeOfEnum != 0)
    {
        return s_sizeOfEnum;
    }

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

        s_sizeOfEnum = vecName.size();
        return s_sizeOfEnum;
    }

    return 0;
}

size_t EnumHelper(const char* pszTypeName, vector<string>& vecNames, const char* pszNames = NULL);
size_t ParseEnum(const char* pszTypeName, const char* pszNames);
string GetEnumTypeName(const string& strFuncSig);

template<typename T>
string GetEnumName(T enumValue, const std::function<char(char)> processor = nullptr)
{
    static_assert(std::is_enum_v<T>, __FUNCTION__ "'s enumValue need a enum");

    static map<T, string> s_mapName;

    if ((unsigned)enumValue == 0)
    {
        return "";
    }

    if (!s_mapName.count((T)0))
    {
		const string& strEnumTypeName = GetEnumTypeName(__FUNCSIG__);
		vector<string> vecNames;
		EnumHelper(strEnumTypeName.c_str(), vecNames);
        s_mapName[(T)0] = "placeholder";
		for (int i = 0; i < vecNames.size(); ++i)
		{
			s_mapName[(T)(i + 1)] = vecNames[i];
		}
    }

	string res;
	auto it = s_mapName.find(enumValue);
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
//#define ENUM_DEFINE(type, ...) enum class type { placeholder, __VA_ARGS__ }; const size_t g_uEnumSizeOf##type = ParseEnum(type::placeholder, #__VA_ARGS__);
#define ENUM_DEFINE(type, ...) enum class type { placeholder, __VA_ARGS__ }; const size_t g_uEnumSizeOf##type = ParseEnum(#type, #__VA_ARGS__);