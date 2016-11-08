#ifndef __LEXER_H
#define __LEXER_H
#include <vector>
#include <string>
#include "token.h"

typedef std::vector<Token>  TokenList; // context tokenlist 
typedef TokenList::iterator TokenIt; // tokenLists' iterator 

class Lexer
{
public:
    static Lexer* get();
    void tokenLize(std::string& code);
    TokenList* getTokenList();

private:
    static Lexer* lexer; 
    Lexer();
    // all tokens 
    TokenList tokenList;

    void dropComments(std::string& code);
    int matchTokenStr(const std::string& c, int i);
    int matchSeparator(const std::string& c, int i);
    // string like "sample string" 
    // return secont quot index 
    int matchString(const std::string& c, int i );
    // check if a string is a pure separator, aka black \n \t
    bool isPureSeparator(const char* s);
};

#endif