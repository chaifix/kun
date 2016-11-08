#include "atom.h"
#include "expression.h"

Expression* Expression::parse(Context* context)
{
    // construct our expression chain(or tree) here
    Token& token = context->tokenList[context->pos];
    return Or::parse(context);
}
/*
// ++, --
Addition::Addition() :op(tNAT){};

Expression* Addition::parse(Context* context)
{
    Addition* add = new Addition();
    add->lexp = Atom::parse(context);

    add->consume(context);
    if (add->op == tNAT)
        return add->lexp;
    else
        return add;
}

Value Addition::evaluate(Context* context)
{
    switch (op)
    {
    case tAdd1:
        return lexp->evaluate(context) + Value(1.f);
    case tSub1:
        return lexp->evaluate(context) - Value(1.f);
    }
}

void Addition::consume(Context* context)
{
    TokenList& tokens = context->tokenList;
    int& pos = context->pos;
    if (tokens[pos].t == tAdd1
     || tokens[pos].t == tSub1)
    {
        op = tokens[pos].t;
        pos++;
    }
}
*/
// *, /
Times::Times(){}

Expression* Times::parse(Context* context)
{
    Times* times = new Times();
    // if there is a atom, we just use it as left operand.
    times->expressions.push_back(Atom::parse(context));
    times->consume(context);
    // if doesnt match a * or /, return a atom.
    if (times->expressions.size() == 1)
        return times->expressions[0];
    // if there is a *, or /, then we return it. 
    else
        return times;
}

Value Times::evaluate(Context* context)
{
    // first, save left operand as base
    Value res = expressions[0]->evaluate(context);
    for (int i = 0; i < operators.size(); i++)
    {
        switch (operators[i])
        {
        case tMul:
            res *= expressions[i + 1]->evaluate(context);
            break;
        case tDiv:
            res /= expressions[i + 1]->evaluate(context);
            break;
        }
    }
    return res;
}

void Times::consume(Context* context)
{
    TokenList& tokens = context->tokenList;
    int& pos = context->pos;
    while (tokens[pos].t == tMul || tokens[pos].t == tDiv) // maybe it's ')'
    {
        // take a operator 
        operators.push_back(tokens[pos].t);
        // take a atom
        pos++;
        expressions.push_back(Atom::parse(context));
    }
}

// +, - 
Plus::Plus(){}

Expression* Plus::parse(Context* context)
{
    Plus* plus = new Plus();
    plus->expressions.push_back(Times::parse(context));
    plus->consume(context);
    if (plus->expressions.size() == 1)
        return plus->expressions[0];
    else
        return plus;
}

Value Plus::evaluate(Context* context)
{
    // first, save left operand as base
    Value res = expressions[0]->evaluate(context);
    for (int i = 0; i < operators.size(); i++)
    {
        switch (operators[i])
        {
        case tPlus:
            res += expressions[i + 1]->evaluate(context);
            break;
        case tMinus:
            res -= expressions[i + 1]->evaluate(context);
            break;
        }
    }
    return res;
}

void Plus::consume(Context* context)
{
    TokenList& tokens = context->tokenList;
    int& pos = context->pos;
    while (tokens[pos].t == tPlus || tokens[pos].t == tMinus)
    {
        // take a operator 
        operators.push_back(tokens[pos].t);
        // take a atom
        pos++;
        expressions.push_back(Times::parse(context));
    }
}

// >, <, ==, >=, <=, != 
Comparison::Comparison():lexp(NULL), rexp(NULL){};

Expression* Comparison::parse(Context* context)
{
    Comparison* comparison = new Comparison();
    comparison->lexp = Plus::parse(context);
    comparison->consume(context);
    if (comparison->rexp == NULL)
        return comparison->lexp;
    else
        return comparison;
}

Value Comparison::evaluate(Context* context)
{
    // first, save left operand as base
    Value lval = lexp->evaluate(context);
    Value rval = rexp->evaluate(context);
    switch (op)
    {
    case tGt:
        return lval > rval;
    case tLt:
        return lval < rval;
    case tNgt:
        return lval <= rval;
    case tNlt:
        return lval >= rval;
    case tEq:
        return lval == rval;
    case tNeq:
        return !(lval == rval);
    }
}

void Comparison::consume(Context* context)
{
    TokenList& tokens = context->tokenList;
    int& pos = context->pos;
    if (tokens[pos].t == tGt 
        || tokens[pos].t == tLt
        || tokens[pos].t == tNgt
        || tokens[pos].t == tNlt
        || tokens[pos].t == tEq
        || tokens[pos].t == tNeq)
    {
        // take a operator 
        op = tokens[pos].t;
        // take a atom
        pos++;
        rexp = Plus::parse(context);
    }
}

// and 
And::And(){};

Expression* And::parse(Context* context)
{
    And* and = new And();
    and->lexp = Comparison::parse(context);
    and->consume(context);
    if (and->rexp == NULL)
        return and->lexp;
    else
        return and;
}

Value And::evaluate(Context* context)
{
    // first, save left operand as base
    Value lval = lexp->evaluate(context);
    Value rval = rexp->evaluate(context);
    return lval && rval;
}

void And::consume(Context* context)
{
    TokenList& tokens = context->tokenList;
    int& pos = context->pos;
    if (tokens[pos].t == tAnd)
    {
        // take a operator 
        op = tokens[pos].t;
        // take a atom
        pos++;
        rexp = Comparison::parse(context);
    }
}

// or 
Or::Or(){};

Expression* Or::parse(Context* context)
{
    Or* or = new Or();
    or->lexp = And::parse(context);
    or->consume(context);
    if (or->rexp == NULL)
        return or->lexp;
    else
        return or;
}

Value Or::evaluate(Context* context)
{
    // first, save left operand as base
    Value lval = lexp->evaluate(context);
    Value rval = rexp->evaluate(context);
    return lval || rval;
}

void Or::consume(Context* context)
{
    TokenList& tokens = context->tokenList;
    int& pos = context->pos;
    if (tokens[pos].t == tOr)
    {
        // take a operator 
        op = tokens[pos].t;
        // take a atom
        pos++;
        rexp = And::parse(context);
    }
}