#pragma once

#include <string>

// 编译期字符串结构体（通过buffer和size确定内容）
struct CompilerStr
{
    const char* buffer;         
    int size;

    constexpr CompilerStr() :
        buffer(nullptr), size(0)
    {}

    constexpr void FillData(const char* buffer, int size)
    {
        this->buffer = buffer;
        this->size = size;
    }

    std::string ToString() const
    {
        if (buffer == nullptr || size == 0)
        {
            return "";
        }
        return std::string(buffer, size);
    }
};

// 编译期字符串数组
template<int N>
struct CompilerStrs
{
    CompilerStr values[N];

    constexpr CompilerStrs() {}

    std::string GetStr(int index) const
    {
        if (index < 0 || index >= N)
        {
            return "";
        }
        return values[index].ToString();
    }
};

// 编译期获取字符串长度
constexpr int CompilerStrlen(const char* buffer)
{
    int i = 0;
    for (; buffer[i]; ++i) {}
    return i;
}

// 编译期切分枚举名列表（x, y, z ...）
template<int N>
constexpr CompilerStrs<N> CompilerSplitEnumNames(const char* buffer)
{
    CompilerStrs<N> res;

    int index = 0;  // 下一个枚举名的下标
    int offset = -1;// 下一个枚举名的起始位置
    for (int i = 0; index < N && buffer[i]; ++i)
    {
        if (buffer[i] == ',')
        {
            if (offset >= 0 && offset < i)
            {
                res.values[index++].FillData(buffer + offset, i - offset);
            }
            offset = -1;
        }
        else if (offset == -1 && buffer[i] != ' ')
        {
            offset = i;
        }
    }

    if (index < N && offset != -1)
    {
        int len = CompilerStrlen(buffer);
   
        if (offset >= 0 && offset < len)
        {
            res.values[index++].FillData(buffer + offset, len - offset);
        }
    }

    return res;
}

// 编译期获得枚举名列表的枚举数量
constexpr int CompilerGetEnumNamesNum(const char* buffer)
{
    int num = 0;
    for (int i = 0; buffer[i]; ++i)
    {
        if (buffer[i] == ',' && buffer[i+1] != 0)
        {
            ++num;
        }
    }
    return num + 1;
}

// 获取枚举名的模板函数
template<typename T>
inline std::string GetEnumName(T enumValue)
{
    return "";
}

/*
ENUM_DEFINE 用法示例：
ENUM_DEFINE ( Color,
    Red,
    Blue,
)

GetEnumName(Color::Red) -> "Red"
GetEnumName(Color::Red, std::toupper) -> "RED"

注意点：
1、枚举值只能系统自增，不能写 Red = 1，代码未处理该种情况
*/
#define ENUM_DEFINE(type, ...) enum class type { __VA_ARGS__ }; \
    constexpr int ENUM_SIZE_OF_##type = CompilerGetEnumNamesNum(#__VA_ARGS__); \
    constexpr CompilerStrs<ENUM_SIZE_OF_##type> ENUM_NAMES_OF_##type = CompilerSplitEnumNames<ENUM_SIZE_OF_##type>(#__VA_ARGS__);\
    template<> \
    inline std::string GetEnumName<type>(type enumValue) \
    {\
        return ENUM_NAMES_OF_##type.GetStr((int)enumValue);\
    }
