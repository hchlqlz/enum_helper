#include "EnumHelper.h"

using namespace std;

vector<string> split(const string& str, const string& sep)
{
    vector<string> ret;
    for (string::size_type startPos = 0U; startPos < str.size();)
    {
        string::size_type endPos = str.find(sep, startPos);
        if (endPos == string::npos)
        {
            endPos = str.size();
        }

        if (endPos - startPos == 0)
        {
            ret.push_back("");
        }
        else
        {
            ret.push_back(str.substr(startPos, endPos - startPos));
        }

        startPos = endPos + sep.size();
    }

    return ret;
}


size_t ParseEnum(const char* pszTypeName, const char* pszNames)
{
	if (pszTypeName == NULL
		|| pszNames == NULL)
	{
		return 0;
	}

	if (nullptr != pszNames)
	{
		vector<string> vecNames = split(pszNames, ",");
		return EnumHelper(pszTypeName, vecNames, pszNames);
	}

	return 0;
}


string GetEnumTypeName(const string& strFuncSig)
{
    const char KEYWORD[] = "<enum ";
    const size_t SIZEOF_KEYWORD = sizeof(KEYWORD) / sizeof(char) - 1;

    size_t posBegin = strFuncSig.find(KEYWORD);
    if (posBegin == string::npos
        || posBegin + SIZEOF_KEYWORD >= strFuncSig.size())
    {
        return "";
    }
    
    size_t posEnd = strFuncSig.find_first_of('>', posBegin);
    if (posEnd == string::npos
        || posEnd <= posBegin + SIZEOF_KEYWORD)
    {
        return "";
    }

    return strFuncSig.substr(posBegin + SIZEOF_KEYWORD, posEnd - posBegin - SIZEOF_KEYWORD);
}

typedef map<int, string> ENUM_VALUE_NAME_MAP;
typedef map<string, ENUM_VALUE_NAME_MAP> ENUM_TYPE_MAP;
ENUM_TYPE_MAP g_mapEnumType;

size_t EnumHelper(const char* pszTypeName, vector<string>& vecNames, const char* pszNames)
{
    if (pszTypeName == NULL)
    {
        return 0;
    }

    if (pszNames != NULL)
    {
        // 避免重复调用的开销
        if (g_mapEnumType.count(pszTypeName))
        {
            return g_mapEnumType[pszTypeName].size();
        }

		for (int i = 0; i < vecNames.size(); ++i)
		{
			if (vecNames.at(i).size() > 0)
			{
                g_mapEnumType[pszTypeName][i + 1] = vecNames.at(i).c_str() + (i == 0 ? 0 : 1);
			}
		}
    }
    else
    {
		auto itType = g_mapEnumType.find(pszTypeName);
		if (itType == g_mapEnumType.end())
		{
			return 0;
		}

		const ENUM_VALUE_NAME_MAP& mapName = itType->second;
		for (auto&& pair : mapName)
		{
			vecNames.push_back(pair.second);
		}
    }

    return vecNames.size();
}
