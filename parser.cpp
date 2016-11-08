#include <iostream>
#include "parser.h"
#include "statement.h"

using namespace std;
// context
Context::Context(TokenList& tokenList) : tokenList(tokenList)
{
    //expressions = ExpList();
    statements = StmtList();
    variables = VarList();
    functions = FuncList();
    //scope = sGol;
    lastPos = std::stack<int>();
}

bool Context::hasVariable(std::string variable)
{
    return variables.count(variable) > 0;
}

void Context::setVariable(std::string variable, Value v)
{
    variables[variable] = v;
}

Value Context::getVariable(std::string variable)
{
    if (variables.count(variable) > 0)
    {
        return variables[variable];
    }
    else
        return (float)0;
}
//
//void Context::setScope(int scope)
//{
//    this->scope = scope;
//}

void Context::pushFunc(Function* func)
{
    funcStack.push(func);
}

void Context::popFunc()
{
    funcStack.pop();
}

Function* Context::topFunc()
{
    if (funcStack.size() == 0) return NULL;
    return funcStack.top();
}

// construct parsing context 
Parser* Parser::parser = NULL;
Parser* Parser::get()
{
    return parser != NULL ? parser : new Parser();
}

void Parser::parse(TokenList& tokenlist)
{
    // save context 
    context = new Context(tokenlist);
    TokenList& tokens = context->tokenList;
    int& pos = context->pos;
    while (tokens[pos].t != tEOF)
    {
        context->statements.push_back(Statement::parse(context));
    }
}

void Parser::execute()
{
    for (Statement* s : context->statements)
    {
        s->executeEx(context);
    }
}