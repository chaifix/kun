#ifndef __SYNTAX_H
#define __SYNTAX_H
#include "token.h"
#include <exception>

// maybe derive from std::runtimr_error is better.
class syntaxError : public std::exception
{
public:
    syntaxError(std::string str);
    const char* what()const override;
private:
    std::string msg;
};
namespace syntaxCheck
{
void check(const Token& t, tid id);

}

#endif