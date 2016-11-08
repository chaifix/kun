#include <ctype.h>
#include <string>
#include <sstream>
#include "util.h"

string loadSource(const string& file)
{

    ifstream t(file);
    string str((istreambuf_iterator<char>(t)),
        istreambuf_iterator<char>());

    return str;
}

bool isFloat( string myString ) 
{
    std::istringstream iss(myString);
    float f;
    iss /*>> noskipws */>> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail(); 
}

float strToFloat(std::string s)
{
    float temp = ::atof(s.c_str());

    return temp;
}

std::string floatToStr(float f)
{
    std::ostringstream ss;
    ss << f;
    std::string s(ss.str());
    return s;
}

struct textToCstringMatche {
    string text;
    string cstring;
};

const textToCstringMatche ttcMatches[] = {
    "\\n", "\n",
    "\\t", "\t",
    "\\\"", "\"",
    "\\\\", "\\",
    "\\\'", "\'",
};

string textToCString(string text)
{
    for (int i = 0; i < text.size(); i++)
    {
        for (int j = 0; j < sizeof(ttcMatches) / sizeof(textToCstringMatche);j++)
        {
            int len = ttcMatches[j].text.size();
            int cstrLen = ttcMatches[j].cstring.size();
            if (text.substr(i, len) == ttcMatches[j].text)
            {
                string sub1 = text.substr(0, i);
                string sub2 = text.substr(i + len, text.size() - i - len);
                text = sub1 + ttcMatches[j].cstring + sub2;
                i += cstrLen - 1;
                break;
            }
        }
    }
    return text;
}