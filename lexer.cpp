#include <iostream>
#include <vector>
#include "lexer.h"
#include "util.h"

using std::vector;
using std::cout;
using std::endl; 
using std::string;

// all tokens with names
const TokenStr tokenStrs[] = {
    tPlus, "+",
    tMinus, "-",
    tMul, "*",
    tDiv, "/",
    tAssign, ":=",
    tAdd, "+=",
    tSub, "-=",
    tAdd1, "++",
    tSub1, "--",

    tAnd, "and",
    tOr, "or",
    tNot, "not",

    tGt, ">",
    tLt, "<",
    tEq, "==",
    tNlt, ">=",
    tNgt, "<=",
    tNeq, "!=",

    tLbracket, "(",
    tRbracket, ")",
    tComma, ",",
    /*tSemi, ";",*/
    //tQuot, "\"",

    tTrue, "true",
    tFalse, "false",
    tNull, "null",

    tIf, "if",
    tThen, "then",
    tElseif, "elseif",
    tElse, "else",
    tEndif, "endif",

    tDo, "do",
    tWhile, "while",
    tEndwhile, "endwhile",
    tFor, "for",
    tEndfor, "endfor",

    tFunc, "func",
    tEndfunc, "endfunc",
    tReturn, "return",

    tPrint, "print",

    tEOF, NULL,
    tNAT, NULL,
};

std::string getTokenStr(tid t)
{
    if (t == tNum)
        return "number";
    if (t == tStr)
        return "string";
    if (t == tVar)
        return "identifier";
    for (int i = 0; i < sizeof(tokenStrs) / sizeof(TokenStr); i++)
        if (tokenStrs[i].t == t)
            return tokenStrs[i].str;
    return "";
}

const char* separators[] = {
    " ", "\n", "\t", ";",// pure separators 
    ",", "+", "-", "*", "/", "(", ")", ":=", "+=", "-=", ">", "<", "==", ">=", "<=", "!=", "\""/*, ";",*/
};

Lexer* Lexer::lexer = NULL;

Lexer* Lexer::get()
{
    return (lexer != NULL) ? lexer : new Lexer();
}

void Lexer::dropComments(string& code)
{
    string::iterator it;
    string::iterator it2;
    for (it = code.begin(); it != code.end(); it++)
    {
        if ((*it) == '/')
        {
            it2 = it + 1;
            if ((*it2) == '/')
            {
                it2++;
                if ((*it2) == '-')
                {
                    for (++it2; it2 < code.end(); it2++)
                    {
                        if ((*it2) == '-' && (*(it2 + 1)) == '/' && (*(it2 + 2)) == '/')
                            break;
                    }
                    it = code.erase(it, it2 + 3) - 1;
                }
                else
                {
                    for (it2; it2 != code.end(); it2++)
                    {
                        if ((*it2) == '\n')
                        {
                            it = code.erase(it, it2 + 1) - 1;
                            break;
                        }
                        if (it2 == code.end() - 1)
                        {
                            it = code.erase(it, it2 + 1) - 1;
                            break;
                        }
                    }
                }
            }
        }
    }
}

int Lexer::matchTokenStr(const string& code, int i)
{
    // all matches 
    vector<int> maches = {};
    for (int t = 0; t < sizeof(tokenStrs) / sizeof(TokenStr); t++)
    {
        TokenStr token = tokenStrs[t];
        if (token.str != NULL)
            if (strcmp(code.substr(i, strlen(token.str)).c_str(), token.str) == 0)
                maches.push_back(t);
    }
    // there is a match 
    if (maches.size() > 0)
    {
        int index = maches[0];
        if (maches.size() > 1)
        {
            int len = strlen(tokenStrs[0].str);
            for (int v : maches)
                if (len < strlen(tokenStrs[v].str))
                {
                    index = v;
                    len = strlen(tokenStrs[v].str);
                }
        }
        // check if comes a negetive number, like -3.1
        //if (tokenStrs[index].str == "-" && code[i+1])

        return index;
    }
    return -1;
}

int Lexer::matchSeparator(const std::string& code, int i)
{
    vector<int> matches;
    int separatorsSize = sizeof(separators) / sizeof(separators[0]);
    for (int s = 0; s < separatorsSize; s++)
    {
        const char* str = separators[s];
        int len = strlen(str);
        if (strcmp(code.substr(i, len).c_str(), str) == 0)
            matches.push_back(s);
    }
    // there is a match 
    if (matches.size() > 0)
    {
        int index = matches[0];
        if (matches.size() > 1)
        {
            int len = strlen(separators[0]);
            for (int v : matches)
                if (len < strlen(separators[v]))
                {
                    index = v;
                    len = strlen(separators[v]);
                }
        }
        return index;
    }
    return -1;
}

int Lexer::matchString(const std::string& c, int i)
{
    if (c[i] == '\"')
    {
        for (int j = i + 1; j < c.size(); j++)
        {
            if (c[j] == '\"' && c[j-1] != '\\')
                return j;
        }
    }
    // doesn't match a string 
    return -1;
}

bool Lexer::isPureSeparator(const char* s)
{
    if (strcmp(" ", s) == 0 ||
        strcmp("\t", s) == 0 ||
        strcmp("\n", s) == 0 ||
        strcmp(";", s) == 0)

        return true;
    return false;
}

void Lexer::tokenLize(string& code)
{
    dropComments(code);
    //cout << code << endl << "----------------------" << endl;
    for (int i = 0; i < code.size(); )
    {
        // 1. check if it match a string like "sample string"
        int quotIndex = matchString(code, i); 
        if (quotIndex != -1)
        {
            int len = quotIndex - i - 1;
            Token token = Token(tStr, textToCString(code.substr(i + 1, len)).c_str());
            tokenList.push_back(token);
            i = quotIndex + 1;
            continue;
        }
        // 2. match token successfully
        int tokenIndex = matchTokenStr(code, i);
        if (tokenIndex != -1)
        {
            int len = strlen(tokenStrs[tokenIndex].str);
            Token token = Token(tokenStrs[tokenIndex]);
            tokenList.push_back(token);
            //tokenList.push_back(Token(tokenStrs[tokenIndex].t));
            //cout << tokenStrs[tokenIndex].str << endl;
            i += len;
            continue;
        }
        // 3. if it is a seperater 
        int separaterIndex = matchSeparator(code, i);
        if (separaterIndex != -1)
        {
            int len = strlen(separators[separaterIndex]);
            const char* separater = separators[separaterIndex];
            if (isPureSeparator(separater))
            {
                // drop all space, \t, \n 
                i += len;
                continue;
            }
        }
        // 4. number or variables or functions 
        // find variable length 
        for (int j = i + 1; j < code.size(); j++)
        {
            if (matchSeparator(code, j) != -1)
            {
                string value = code.substr(i, j - i);
                if (isFloat(value))
                {
                    Token token = Token(tNum, strToFloat(value));
                    tokenList.push_back(token);
                }
                else
                {
                    Token token = Token(tVar, value.c_str());
                    tokenList.push_back(token);
                }

                i += j - i;
                break;
            }
        }
    }
    tokenList.push_back(Token(tEOF, ""));
    /*
    for (auto v : tokenList)
    {
        tid t = v.t;
        if (t == tNum)
        {
            cout << v.n << ',';
            cout << endl;
        }
        else if (t != tEOF)
        {
            cout << v.s << ',';
            cout << endl;
        }
    }
    */
}

Lexer::Lexer()
{
}

TokenList* Lexer::getTokenList()
{
    return &tokenList;
}