#include "value.h"
#include "expression.h"
/*
Atom is the basic unit of the expression, such as
    identifiers,
    literals,
aka
    variable name,
    string value, number value, boolean value
*/
class Atom : public Expression
{
public:
    static Expression* Atom::parse(Context* context);
    virtual Value evaluate(Context* context) = 0;
};

// +, -
class Addition : public Atom
{
public:
    Addition(tid t) :op(t){};
    Value evaluate(Context* context){ return 0.f; };
    tid op;
};

// handle all variables
class Variable : public Atom
{
public:
    Variable(string varName, Context* context);
    Value evaluate(Context* context);
    string getName();
private:
    string  name;
    // ++, --
    Addition* op;
};

// handle all fixed string value or number value 
class Literal : public Atom
{
public:
    Literal(Value v);
    Value evaluate(Context* context);
private:
    Value value;
};

// not 
class Not : public Atom
{
public:
    Not(Expression* exp);
    Value evaluate(Context* context);
private:
    Expression* expression;
};

// "negtive" a value
class Negtive : public Atom
{
public:
    Negtive(Expression* exp);
    Value evaluate(Context* context);
private:
    Expression* expression;
};

// just like not 
class Bracket : public Atom
{
public:
    Bracket(Expression* exp);
    Value evaluate(Context* context);
private:
    Expression* expression;
};

// calling a function then get it's return value 
class FuncCalling :public Atom
{
public:
    FuncCalling(Context* context);
    Value evaluate(Context* context);
    static Expression* parse(Context* context);
private:
    void consume(Context* context);
    std::vector<Expression*> arguments;
    std::string func;
};