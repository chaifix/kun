#include "token.h"
using std::string;

Token::Token()
{
    t = tNAT;
}

Token::Token(const TokenStr& tok)
{
    t = tok.t;
    s = tok.str;
}

Token::Token(tid t)
{
    this->t = t;
}

// tNum
Token::Token(tid t, float n) : t(t), n(n)
{
}

// tStr 
Token::Token(tid t, char const* s) : t(t), s(s)
{
}

Token::~Token()
{
}

Token::Token(const Token& t)
{
   // printf("ERROR: trying to call Token(const Token& t)\n");
    this->t = t.t;
    switch (this->t)
    {
    case tNum:
        this->n = t.n;
        break;
    default: 
        this->s = t.s;
    }
}