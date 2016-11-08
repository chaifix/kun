#ifndef __STATEMENT_H
#define __STATEMENT_H

#include <iostream>
#include "lexer.h"
#include "expression.h"
#include "parser.h"

using std::cout;
class Function;
/*
recursive-descent parser
*/
class Statement
{
public:
    Statement();
    void executeEx(Context* context);
    static Statement* parse(Context* context);
protected:
    virtual void execute(Context* context) = 0;
    static bool term(tid eExpected, const Token& t)
    {
        if (eExpected != t.t)
            return false;
        return true;
    }
};

// a statements block 
class Block : public Statement
{
public:
    Block();
    // stop parsing when match given end token type
    Block(int end, Context* context);
    void execute(Context* context);
private:
    StmtList statements;
    int endFlag;
};

typedef std::vector<Block*> BlockList;

// print("hello, world")
class PrintStmt : public Statement
{
public:
    PrintStmt();
    PrintStmt(Context* context);
    void execute(Context* context);
private:
    Expression* expression;
};

/*
if <expression> then 
    <statement>
elseif <expression> then 
    <statement>
else 
    <statement>
endif
*/
class IfStmt : public Statement
{
public:
    IfStmt(Context* context);
    void execute(Context* context);
private:
    // each branch has a condition expression
    ExpList   conditions;
    BlockList blocks;
};

/*
while <expression> do 
    <statement>
endwhile
*/
class WhileStmt: public Statement
{
public:
    WhileStmt(Context* context);
    void execute(Context* context);
private:
    Expression* condition;
    Block*      block;
};

/*
a := <expression>
*/
class AssignStmt: public Statement
{
public:
    AssignStmt(Context* context);
    void execute(Context* context);
private:
    string variableName;
    // right side 
    Expression* expression;
};

/*
a ++
*/
class AddOne : public Statement
{
public:
    AddOne(Context* context);
    void execute(Context* context);
private:
    string variableName;
    // right side 
};
/*
a --
*/
class SubOne : public Statement
{
public:
    SubOne(Context* context);
    void execute(Context* context);
private:
    string variableName;
    // right side 
};

/*
<variable> += <expression>
*/
class Add : public Statement
{
public:
    Add(Context* context);
    void execute(Context* context);
private:
    string variableName;
    // right side 
    Expression* expression;
};

/*
<variable> -= <expression>
*/
class Sub : public Statement
{
public:
    Sub(Context* context);
    void execute(Context* context);
private:
    string variableName;
    // right side 
    Expression* expression;
};

/*
    for <variable> = <expression>, <expression>[, <expression>] do 
        <statement>
    endfor
*/
class ForStmt : public Statement
{
public:
    ForStmt(Context* context);
    void execute(Context* context);
private:
    std::string variable;
    float       start;
    float       end;
    float       step;
    Block*      block;
};

/*
    [<variable> = ] <function name> ([<express1>[, <expression2>[, ...]]])
*/
class FuncCallStmt : public Statement
{
public:
    FuncCallStmt(Context* context);
    void execute(Context* context);
private :
    std::vector<Expression*> arguments;
    string  func;
};

/*
    return <expression>
*/
class FuncReturn : public Statement
{
public:
    FuncReturn(Context* context);
    void execute(Context* context);
private:
    Expression* exp;
};

#endif