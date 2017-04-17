#include <iostream>

#include "JackAnalyzer.h"
#include "JackTokenizer.h"
#include "CompilationEngine.h"

using namespace std;
using namespace JackCompiler;

void TestTokenizer(const std::string& input)
{
    JackTokenizer tokenizer(input);

    while (tokenizer.HasMoreTokens())
    {
        tokenizer.Advance();
        eTokenType tokenType = tokenizer.GetTokenType();
        if (tokenType == eTokenTypeKeyword)
        {
            cout << "Keyword: " << tokenizer.GetKeyword() << endl;
        }
        else if (tokenType == eTokenTypeStringConst)
        {
            cout << "String const: " << tokenizer.GetStringVal() << endl;
        }
        else if (tokenType == eTokenTypeIntConst)
        {
            cout << "Int value: " << tokenizer.GetIntVal() << endl;
        }
        else if (tokenType == eTokenTypeIdentifier)
        {
            cout << "Identifier: " << tokenizer.GetIdentifier() << endl;
        }
        else if (tokenType == eTokenTypeSymbol)
        {
            cout << "Symbol: " << tokenizer.GetSymbol() << endl;
        }
    }
}

void TestCompilationEngine(const std::string& input, const std::string& output)
{
    CompilationEngine compEng(input, output);
    compEng.CompileFile();
}

int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        cerr << "usage: " << argv[0] << " <input file>.jack" << endl;
        return 1;
    }

    //TestTokenizer(argv[1]);
    //TestTokenizer("bin/Debug/input.jack");
    //TestCompilationEngine("bin/Debug/basic-class.jack", "bin/Debug/basic-class.xml");
    //TestCompilationEngine("bin/Debug/Main.jack", "bin/Debug/output.xml");

    JackAnalyzer jackAna;
    string filename(argv[1]);
    size_t dotIndex = filename.find_last_of(".");

    if (dotIndex != string::npos)
    {
        jackAna.CompileFile(filename);
    }
    else
    {
        jackAna.CompileDirectory(filename);
    }

    return 0;
}
