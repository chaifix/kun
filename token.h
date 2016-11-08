#ifndef __TOKEN_H
#define __TOKEN_H
#include <string>

// token id 
enum tid
{   
    // end of statement block 
    tEndwhile = 2,
    tEndfunc  = 4,
    tEndfor   = 8,
    tEOF      = 16,
    tElseif   = 32,
    tEndif    = 64,
    tElse     = 128,
    // function or variable name 
    tVar,
    // values 
    tNum, tStr,

    // operators 
    tPlus, tMinus, tMul, tDiv,
    tAssign,
    tAdd, tSub, tAdd1, tSub1,
    tAnd, tOr, tNot,
    tGt, tLt, tEq, tNlt, tNgt, tNeq,
    tTrue, tFalse, tNull,
    // ( ) , ;
    tLbracket, tRbracket, 
    tComma, /*tQuot,*//* tSemi,*/
    // if statement 
    tIf, tThen, 
    // loop statements 
    tDo,
    // 1. while statement 
    tWhile, 
    // 2. for loop statement 
    tFor, 
    // function 
    tFunc, 
    tReturn,
    // util functions 
    tPrint,
    // unknow token, aka not a token
    tNAT,
};

struct TokenStr
{
    tid t;
    const char* str;
};

class Token
{
public:
    Token();
    Token(const TokenStr&);
    Token(const Token& t);
    Token(tid t);
    // tNum
    Token(tid t, float n);
    // tStr 
    Token(tid t, char const* s);
    ~Token();
   
    tid t;
    // if this token is a tNum
    float n; 
    // if this token is a tStr
    std::string s;
};
// defined in lexer.cpp
extern std::string getTokenStr(tid t);

#endif