#ifndef __EXPRESSION_H
#define __EXPRESSION_H
#include <vector>
#include "token.h"
#include "value.h"
#include "parser.h"
using std::string;
//using std::list;
/*
A expression is calculating unit, must return a value. 
*/
class Expression
{
public:
    virtual Value evaluate(Context* context) = 0;
    static Expression* parse(Context* context);
protected:
    typedef std::vector<int> OpList;
};
/*
// ++, --
class Addition : Expression
{
public:
    Addition();
    Value evaluate(Context* context);
    static Expression* parse(Context* context);
private:
    void consume(Context* context);
    Expression* lexp;
    tid         op;
};
*/
// *, /
class Times : Expression
{
public:
    Times();
    Value evaluate(Context* context);
    static Expression* parse(Context* context);
private :
    // Keep consuming all * and / because
    // of the same priority, we dont need to 
    // go through the expression constrctor 
    // chain again.
    void consume(Context* context);
    ExpList expressions;
    OpList  operators;
};

// plus and minus have the same priority
class Plus : Expression
{
public:
    Plus();
    Value evaluate(Context* context);
    static Expression* parse(Context* context);
private:
    void consume(Context* context);
    ExpList expressions;
    OpList  operators;
};

// greater, less, equal, not less, not greater, and not equal 
// have the same priority
class Comparison : Expression
{
public:
    Comparison();
    Value evaluate(Context* context);
    static Expression* parse(Context* context);
private:
    void consume(Context* context);
    Expression* lexp;
    Expression* rexp;
    tid         op;
};

// and 
class And : Expression
{
public:
    And();
    Value evaluate(Context* context);
    static Expression* parse(Context* context);
private:
    void consume(Context* context);
    Expression* lexp;
    Expression* rexp;
    tid         op;
};

// or
class Or : Expression
{
public:
    Or();
    Value evaluate(Context* context);
    static Expression* parse(Context* context);
private:
    void consume(Context* context);
    Expression* lexp;
    Expression* rexp;
    tid         op;
};

#endif