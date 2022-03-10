#pragma once

#include <map>
#include <algorithm>
#include <string>
#include <vector>
#include <functional>

std::vector<std::string> split(const std::string& str, const std::string& sep);

template<typename T>
std::string EnumHelper(T key, const std::function<char(char)> processor = nullptr, const char* pszName = NULL)
{
    static_assert(std::is_enum_v<T>, __FUNCTION__ "'s key need a enum");

    static std::map<T, std::string> s_mapName;

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

    // �����ظ����õĿ���
    if (s_sizeOfEnum != 0)
    {
        return s_sizeOfEnum;
    }

    if (nullptr != pszNames)
    {
        const std::vector<std::string>& vecName = split(pszNames, ",");
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

/*
ENUM_DEFINE �÷�ʾ����
ENUM_DEFINE ( Color,
    Red,
    Blue,
)

EnumHelper(Color::Red) -> "Red"
EnumHelper(Color::Red, std::toupper) -> "RED"

ע��㣺
1��ö��ֵֻ��ϵͳ����������д Red = 1������δ����������
*/
#define ENUM_DEFINE(type, ...) enum class type { placeholder, __VA_ARGS__ }; const size_t g_uEnumSizeOf##type = ParseEnum(type::placeholder, #__VA_ARGS__);