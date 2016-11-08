#include <iostream>
#include "statement.h"
#include <string>
#include "syntax.h"
#include "util.h"
#include "lexer.h"
#include "parser.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "kun:   No file is given. Usage:\n";
        cout << "       kun <script>\n";
        return 1;
    }

    string file = argv[1];
    ifstream t(file);
    if (t)
    {
        Lexer* lexer = Lexer::get();
        //std::string s = loadSource(file);
        string s((istreambuf_iterator<char>(t)),
            istreambuf_iterator<char>());
        lexer->tokenLize(s);
        Parser* parser = Parser::get();
        try
        {
            parser->parse(*lexer->getTokenList());
            parser->execute();
        }
        catch (std::exception & ept)
        {
            cout << ept.what() << endl;
        }
    }
    else
    {
        cout << "kun:   No such file or directory.\n";
        return 1;
    }

    return 0;
}