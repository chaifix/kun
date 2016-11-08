#ifndef __PARSER_H
#define __PARSER_H
#include <map>
#include <string>
#include <vector>
#include <stack>
#include "lexer.h"
#include "value.h"
#include "token.h"

class Function;

class Expression;
class Statement;

typedef std::vector<Expression*> ExpList;
typedef std::vector<Statement*> StmtList;
typedef std::map<std::string, Value> VarList;
typedef std::map<std::string, Function*> FuncList;

class Context
{
public:
    /*
    enum envScope                  
    {
        sGol  = 2, 
        sFunc = 4,
    };
    */
    //void setScope(int scope);

    Context();
    Context(TokenList& tokenList);

    //int scope;                  // statement running env.

    TokenList& tokenList;        // all tokens to parse 
    //TokenIt tokenIt;           // token iterator 
    int pos;                     // current position 
    //ExpList expressions;         // all expressions
    StmtList statements;         // all statements
    VarList variables;           // variable name -> value 
    FuncList functions;          // function name -> function defination position 
    std::stack<Function*> funcStack; //  
    std::stack<int> lastPos;     // when we do function calls, we push the current position onto a stack.
	                             // This way if we have nested function calls we can unwind the stack
	                             // properly.
    //static string operators[]; // all operators may used 

    void setVariable(std::string variable, Value v); 
    bool hasVariable(std::string variable);
    Value getVariable(std::string variable);

    void pushFunc(Function* func);
    void popFunc();
    Function* topFunc();
};

class Parser
{
public:
    static Parser* get();
    void parse(TokenList& tokenlist);
    void execute();
private:
    Parser(){};
    static Parser* parser;
    Context* context;
};

#endif