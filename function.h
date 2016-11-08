#ifndef __FUNCTION_H
#define __FUNCTION_H
#include <string>
#include <list>
#include <stack>
#include "parser.h"
#include "statement.h"
/*
func <function name> ( [<argument1>[, <argument2>[, ...]]] )
    <statement>
    [return <expression>]
endfunc
*/
typedef std::vector<Value> Args;
class Block;

class Function : public Statement
{
public:
    Function(Context* context);
    void execute(Context* context);
    Value execute(Context* context, Args& args);

    void  addVar(Token* t); // add a variable to both vars and args 
    //Token* getVar();      // get a variable on top of the stack vars 
    std::string getName();
    int getVarCount();            
    int getPosition();

    Value getVariable(std::string);
    bool hasVariable(std::string name);
    void setVariable(std::string name, Value v);

    bool isReturn();
    Value returnf();        // will empty returns stack 
    void pushReturn(Value v);

private:
    std::string func;          // function name 
    int position;              // position of the statement block start point 
                               // in tokenlist  
    std::stack<Value> returns; // return values 
    VarList variables;         // local variables 
    Block*  block;             
    std::vector<string> slots; // map argument index to it's name 
};

#endif