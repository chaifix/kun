#include "statement.h"
#include "function.h"

Function::Function(Context* context)
{
    TokenList& tokens = context->tokenList;
    int& pos = context->pos;
    // tokens[pos] is func
    // save arguments 
    pos++;
    func = tokens[pos].s;
    pos++; // skip name
    while (tokens[pos++].t != tRbracket)
    {
        if (tokens[pos].t != tRbracket)
        {
            variables[tokens[pos].s] = Value(0.f);
            slots.push_back(tokens[pos].s);
            pos++;
        }
    }
    // token[pos] is function block 
    position = pos;
    block = new Block(tEndfunc, context);
    pos++; // skip endfunc
    // add this function to context.functions
    context->functions[func] = this;
    //
    returns = std::stack<Value>();
}

void Function::execute(Context* context)
{
    // dont run function as definition formula
}

Value Function::execute(Context* context, Args& args)
{
    // reset stacks
    // give every argument a specific value 
    for (int i = 0; i < slots.size(); i++)
    {
        variables[slots[i]] = args[i];
    }
    //context->setScope(Context::sGol | Context::sFunc);
    context->pushFunc(this);
    block->execute(context);
    context->popFunc();
    //context->setScope(Context::sGol); // dont setScope back to sGol here, because of nested calling functions.
                                        // just use context->popFunc
    // function return value 
    if (isReturn())
    {
        Value v = returnf();
        return v;
    }
    else
        return 0.f;
}

void Function::addVar(Token* t)
{
    //vars.push(t);
    //for (Token* v : args)
    //    if (v == t)
    //        return;
    //    else
    //        args.push_back(t);
}
//
//Token* Function::getVar()
//{
//    //Token* v = vars.top();
//    //vars.pop();
//    //if (v != NULL)
//    //    return v;
//    //else
//    //    return NULL;
//    return NULL;
//}

std::string Function::getName()
{
    return func;
}

int Function::getVarCount()
{
    return 1;
  //  return vars.size();
}

int Function::getPosition()
{
    return position;
}
//
//void Function::setPosition(int p)
//{
//    position = p;
//}

bool Function::hasVariable(std::string name)
{
    return variables.count(name) > 0;
}

void Function::setVariable(std::string name, Value v)
{
    variables[name] = v;
}

Value Function::getVariable(std::string name)
{
    if (hasVariable(name))
        return variables[name];
}

bool Function::isReturn()
{
    return returns.size() > 0;
}

Value Function::returnf()
{
    Value v = returns.top();
    std::stack<Value>().swap(returns);
    return v;
}

void Function::pushReturn(Value v)
{
    returns.push(v);
}