#include "statement.h"
#include "function.h"
#include "atom.h"
#include "syntax.h"

using namespace syntaxCheck;

Statement::Statement()
{
}

Statement* Statement::parse(Context* context)
{
    Token& token = context->tokenList[context->pos];
    switch (token.t)
    {
    case tVar:
        // a variable just like a trigger here.
        // unlike expression, we can just make a 
        // instance instead of calling parse()
        switch (context->tokenList[context->pos + 1].t)
        {
            case tAssign:
                return new AssignStmt(context);
            case tAdd:
                return new Add(context);
            case tSub:
                return new Sub(context);
            case tAdd1:
                return new AddOne(context);
            case tSub1:
                return new SubOne(context);
            case tLbracket:
                // a function call 
                return new FuncCallStmt(context);
            default:
                throw syntaxError("Unexpected token " + getTokenStr(context->tokenList[context->pos + 1].t) + '\n');
        }
        break;
    case tPrint:
        return new PrintStmt(context);
        break;
    case tIf:
        return new IfStmt(context);
        break;
    case tWhile:
        return new WhileStmt(context);
    case tFor:
        return new ForStmt(context);
    case tFunc:
        return new Function(context);
    case tReturn:
        return new FuncReturn(context);
        break;
    default:
        throw syntaxError("Unexpected token " + getTokenStr(context->tokenList[context->pos + 1].t) + '\n');
        ;
    }
}

void Statement::executeEx(Context* context)
{
    Function* f = context->topFunc();
    if (f != NULL)
    {
        // if function returns, we just abort it 
        if (f->isReturn())
            return;
    }
    execute(context);
}

// statement block 
Block::Block(){}

Block::Block(int endflags, Context* context) : endFlag(endflags)
{
    TokenList& tokens = context->tokenList;
    int& pos = context->pos;
    // first statement start from pos
    while (((int)tokens[pos].t & endFlag) != (int)tokens[pos].t)
        statements.push_back(Statement::parse(context));
}

void Block::execute(Context* context)
{
    if (statements.size() == 0) return;
    for (Statement* stmt : statements)
    {
        stmt->executeEx(context);
    }
}

// assignment statement 
AssignStmt::AssignStmt(Context* context)
{
    TokenList& tokens = context->tokenList;
    int& pos = context->pos;
    // variable
    variableName = tokens[pos].s;
    check(tokens[++pos], tAssign);
    pos ++; // skip :=
    expression = Expression::parse(context);
}

void AssignStmt::execute(Context* context)
{
    //switch (context->scope)
    //{
    //case Context::sGol:
    //    context->setVariable(variableName, expression->evaluate(context));
    //    break;
    //case Context::sGol | Context::sFunc:
        Function* f = context->topFunc();
        if (f)
        {
            if (f->hasVariable(variableName))
                context->topFunc()->setVariable(variableName, expression->evaluate(context));
            else
            {
                if (context->variables.count(variableName) > 0)
                    context->setVariable(variableName, expression->evaluate(context));
                else 
                    context->topFunc()->setVariable(variableName, expression->evaluate(context));
            }
        }
        //if (context->variables.count(variableName) > 0)
        else 
        {
            context->setVariable(variableName, expression->evaluate(context));
            return;
        }
    //    break;
    //}
}

// add one 
AddOne::AddOne(Context* context)
{
    TokenList& tokens = context->tokenList;
    int& pos = context->pos;
    // variable
    variableName = tokens[pos].s;
    check(tokens[++pos], tAdd1);
    pos ++; // skip ++
}

void AddOne::execute(Context* context)
{
    context->setVariable(variableName, context->getVariable(variableName) + Value(1.f));
}

// sub one 
SubOne::SubOne(Context* context)
{
    TokenList& tokens = context->tokenList;
    int& pos = context->pos;
    // variable
    variableName = tokens[pos].s;
    pos += 2; // skip --
}

void SubOne::execute(Context* context)
{
    context->setVariable(variableName, context->getVariable(variableName) - Value(1.f));
}

// add a number 
Add::Add(Context* context)
{
    TokenList& tokens = context->tokenList;
    int& pos = context->pos;
    // variable
    variableName = tokens[pos].s;
    pos += 2; // skip :=
    expression = Expression::parse(context);
}

void Add::execute(Context* context)
{
    context->setVariable(variableName, expression->evaluate(context) + context->getVariable(variableName));
}

// sub a number 
Sub::Sub(Context* context)
{
    TokenList& tokens = context->tokenList;
    int& pos = context->pos;
    // variable
    variableName = tokens[pos].s;
    pos += 2; // skip :=
    expression = Expression::parse(context);
}

void Sub::execute(Context* context)
{
    context->setVariable(variableName, context->getVariable(variableName) - expression->evaluate(context));
}

// print()
PrintStmt::PrintStmt(Context* context)
{
    TokenList& tokens = context->tokenList;
    int& pos = context->pos;
    check(tokens[pos + 1], tLbracket);
    pos += 2;// skip print(
    // check if value is not given 
    // aka print()
    if (tokens[pos].t == tRbracket)
        expression = new Literal(Value(""));
    else 
        expression = Expression::parse(context);
    check(tokens[pos++], tRbracket);
}

void PrintStmt::execute(Context* context)
{
    Value v = expression->evaluate(context);
    if (v.isStr())
        printf(v.getString().c_str());
    else 
        printf(v.toString().c_str());
}

// if statement 
IfStmt::IfStmt(Context* context)
{
    TokenList& tokens = context->tokenList;
    // token[pos] is if
    int& pos = context->pos;
    while (tokens[pos++].t != tEndif)
    {
        if (tokens[pos - 1].t != tElse)
        {
            conditions.push_back(Expression::parse(context));
            pos++; // skip then
        }
        blocks.push_back(new Block(tEndif | tElseif | tElse, context));
    }
}

void IfStmt::execute(Context* context)
{
    for (int i = 0; i < blocks.size(); i++)
    {
        if (i < conditions.size() && conditions[i]->evaluate(context).getBoolean())
        {
            blocks[i]->execute(context);
            break;
        }
        else if (i >= conditions.size())
        {
            blocks[i]->execute(context);
            break;
        }
    }
}

// while loop 
WhileStmt::WhileStmt(Context* context)
{
    TokenList& tokens = context->tokenList;
    // token[pos] is while
    int& pos = context->pos;
    pos++; 
    condition = Expression::parse(context);
    check(tokens[pos++], tDo);
    block = new Block(tEndwhile,context);
    check(tokens[pos++], tWhile); // skip endwhile
}

void WhileStmt::execute(Context* context)
{
    while (condition->evaluate(context).getBoolean())
    {
        block->execute(context);
    }
}

// for loop
ForStmt::ForStmt(Context* context)
{
    step = 1;
    TokenList& tokens = context->tokenList;
    // token[pos] is for
    int& pos = context->pos;
    pos++;
    // must be a variable 
    if (context->tokenList[pos].t != tVar)
    {
        // error, throw a exception here 
    }
    Variable * var = new Variable(context->tokenList[pos].s, context);
    variable = var->getName();
    pos+=2; // skip :=
    start = Expression::parse(context)->evaluate(context).getFloat();
    // set variable value 
    context->setVariable(variable, Value(start));
    pos++; // skip ,
    end = Expression::parse(context)->evaluate(context).getFloat();
    if (context->tokenList[pos].t == tComma)
    {
        pos++; // skip ,
        // given step 
        step = Expression::parse(context)->evaluate(context).getFloat();
    }
    else if (context->tokenList[pos].t == tDo)
    {
    }
    check(tokens[pos++], tDo); // skip do 
    
    block = new Block(tEndfor, context);

    check(tokens[pos++], tFor); // skip endfor
}

void ForStmt::execute(Context* context)
{
    float s = this->step;
    while (context->getVariable(variable).getFloat() <= end)
    {
        block->execute(context); 
        Value v = context->getVariable(variable);
        context->setVariable(variable, v + Value((float)s)); 
    }
}

// a function call 
FuncCallStmt::FuncCallStmt(Context* context)
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

    // token[pos] is next statement 
}

void FuncCallStmt::execute(Context* context)
{
    std::vector<Value> argsv;
    for (int i = 0; i < arguments.size(); i++)
    {
        argsv.push_back(arguments[i]->evaluate(context));
    }
    context->functions[func]->execute(context, argsv);
}

// function return 
FuncReturn::FuncReturn(Context* context)
{
    TokenList& tokens = context->tokenList;
    int& pos = context->pos;
    // tokens[pos] is return 
    check(tokens[pos++].t, tReturn);
    exp = Expression::parse(context);
};

void FuncReturn::execute(Context* context)
{
    Function* f = context->topFunc();
    if (f != NULL)
    {
        Value v = exp->evaluate(context);
        f->pushReturn(v);
    }
}