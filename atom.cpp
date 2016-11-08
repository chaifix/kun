#include "atom.h"
#include "function.h"
#include "syntax.h"
#include "token.h"

// Atom is the last line of defense, or throw a exception
Expression* Atom::parse(Context* context)
{
    TokenList& tokens = context->tokenList;
    int& pos = context->pos;
    Expression* exp = NULL;
    switch (tokens[pos].t)
    {
    case tTrue:
        pos++;
        return new Literal(true);
    case tFalse:
        pos++;
        return new Literal(false);
    case tNull:
        pos++;
        return new Literal(0.f);
    case tNum:
        return new Literal(tokens[pos++].n);
    case tStr:
        return new Literal(tokens[pos++].s);
    case tVar:
        // check if is a function calling 
        if (tokens[pos + 1].t == tLbracket)
        {
            return new FuncCalling(context);
        }
        return new Variable(tokens[pos++].s, context);
    case tMinus:
        pos++;
        return new Negtive(parse(context));
    case tNot:
        pos++;
        return new Not(parse(context));
    case tLbracket:
        pos++; // skip open bracket
        exp = new Bracket(Expression::parse(context));
        pos++; // skip close bracket, when other expression 
               // like +, -, *, / consumed all operators and 
               // expresssions, pos set to where the close 
               // bracket at. Then we just skip it. Close 
               // bracket is unnecessary in our pasing time.
        return exp;
    default:
        throw syntaxError("unexpected token " + getTokenStr(tokens[pos].t) + '\n');
    }
    // error
    return NULL;
}

// variable 
Variable::Variable(string varName, Context* context)
{
    op = NULL;
    name = varName;
//    if (context->variables.count(name) == 0)
//        context->variables[name] = floatToStr(0);
    //switch (context->scope)
    //{
    //case Context::sGol:
    //    if (context->variables.count(name) == 0)
    //        context->variables[name] = floatToStr(0);
    //    break;
    //case Context::sGol | Context::sFunc:
        Function* f = context->topFunc();
        if (f)
        {
            if (!f->hasVariable(name))
                f->setVariable(name, 0.f);
        }
        else
        {
            if (context->variables.count(name) == 0)
                context->variables[name] = floatToStr(0);
        }
    //    break;
    //}
    // ++ -- 
    switch (context->tokenList[context->pos].t)
    {
    case tAdd1:
        op = new Addition(tAdd1);
        context->pos++;
        break;
    case tSub1:
        op = new Addition(tSub1);
        context->pos++;
        break;
    }
}

Value Variable::evaluate(Context* context)
{
    if (op != NULL)
    {
        switch (op->op)
        {
        case tAdd1:
            context->setVariable(name, context->getVariable(name) + Value(1.f));
            break;
        case tSub1:
            context->setVariable(name, context->getVariable(name) - Value(1.f));
            break;
        default:
            break;
        }
    }
    //switch (context->scope)
    //{
    //case Context::sGol:
    //    if (context->variables.count(name) > 0)
    //        return context->variables[name];
    //    break;
    //case Context::sGol | Context::sFunc:
        Function* f = context->topFunc();
        if (f)
        {
            if (f->hasVariable(name))
                return f->getVariable(name);
        }
        if (context->variables.count(name) > 0)
            return context->variables[name];
    //    break;
    //}
    return (float)0;
}

string Variable::getName()
{
    return name;
}

// literal 
Literal::Literal(Value v)
{
    value = v;
}

Value Literal::evaluate(Context* context)
{
    return value;
}

// not 
Not::Not(Expression* exp) : expression(exp)
{
}

Value Not::evaluate(Context* context)
{
    return !expression->evaluate(context);
}

// negtive 
Negtive::Negtive(Expression* exp) : expression(exp)
{
}

Value Negtive::evaluate(Context* context)
{
    return -expression->evaluate(context);
}

// bracket
Bracket::Bracket(Expression* exp) : expression(exp)
{
}

Value Bracket::evaluate(Context* context)
{
    return expression->evaluate(context);
}

// a function call 
FuncCalling::FuncCalling(Context* context)
{
    TokenList& tokens = context->tokenList;
    int& pos = context->pos;
    // token[pos] is func name 
    func = tokens[pos].s;
    pos++; 
    // read all args
    while (tokens[pos++].t != tRbracket)
    {
        if (tokens[pos].t != tRbracket)
        {
            arguments.push_back(Expression::parse(context));
        }
    }
    // token[pos] is next expression 
}

Value FuncCalling::evaluate(Context* context)
{
    std::vector<Value> argsv;
    for (int i = 0; i < arguments.size(); i++)
    {
        argsv.push_back(arguments[i]->evaluate(context));
    }
    return context->functions[func]->execute(context, argsv);
}