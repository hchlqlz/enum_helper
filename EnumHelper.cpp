#include "EnumHelper.h"

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