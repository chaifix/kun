#include <string>
#include "syntax.h"
#include "token.h"
syntaxError::syntaxError(std::string str)
{
    msg = str;
};

const char* syntaxError::what() const
{
    return msg.c_str();
}
namespace syntaxCheck
{
    void check(const Token& t, tid id)
    {
        if (t.t != id)
        {
            throw syntaxError(std::string("Expect ") + getTokenStr(id) + ", but get a " + getTokenStr(t.t) + ".\n");
        }
    }
}