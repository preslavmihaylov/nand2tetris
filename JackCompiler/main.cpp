#include <iostream>

#include "JackController.h"
#include "JackTokenizer.h"
#include "CompilationEngine.h"
#include "VMWriter.h"
#include "SymbolTable.h"

using namespace std;
using namespace JackCompiler;

//#define DEBUG_BUILD

#ifdef DEBUG_BUILD
void TestSymbolTable()
{
    SymbolTable table;

    table.StartSubroutine();
    table.Define("first", "int", eVariableKindArgument);
    table.Define("second", "int", eVariableKindArgument);
    table.Define("classField1", "char", eVariableKindField);
    table.Define("classField2", "char", eVariableKindField);
    table.Define("staticField1", "char", eVariableKindStatic);
    table.Define("staticField2", "char", eVariableKindStatic);
    table.PrintSymbolTable();

    cout << endl;
    cout << "new subroutine started" << endl;
    table.StartSubroutine();
    table.PrintSymbolTable();

    cout << endl;
    cout << "field types cnt: " << table.VarCount(eVariableKindField) << endl;
    cout << "local types cnt: " << table.VarCount(eVariableKindLocal) << endl;
    cout << "argument types cnt: " << table.VarCount(eVariableKindArgument) << endl;
    cout << endl;

    table.Define("local1", "int", eVariableKindLocal);
    table.Define("local2", "int", eVariableKindLocal);
    table.Define("local2", "int", eVariableKindLocal);
    table.Define("arg1", "int", eVariableKindArgument);
    table.PrintSymbolTable();
    cout << endl;

    cout << "type of local1: " << table.GetTypeOf("local1") << endl;
    cout << "type of staticField1: " << table.GetTypeOf("staticField1") << endl;
    cout << "kind of staticField1: " << table.GetKindString(table.GetKindOf("staticField1")) << endl;
    cout << "kind of classField1: " << table.GetKindString(table.GetKindOf("classField1")) << endl;
    cout << "index of local2: " << table.GetIndexOf("local2") << endl;
    cout << "index of arg1: " << table.GetIndexOf("arg1") << endl;
    cout << "type of NOT_FOUND: " << table.GetTypeOf("NOT_FOUND") << endl;
    cout << "kind of NOT_FOUND: " << table.GetKindOf("NOT_FOUND") << endl;
    cout << "index of NOT_FOUND: " << table.GetIndexOf("NOT_FOUND") << endl;
}

void TestVMWriter()
{
    VMWriter writer("output-test.vm");
    writer.WriteCall("functionName", 5);
    writer.WriteFunction("functionDefinition", 2);

    writer.WriteArithmetic(eVMOperationAdd);
    writer.WriteArithmetic(eVMOperationSub);
    writer.WriteArithmetic(eVMOperationNeg);
    writer.WriteArithmetic(eVMOperationEq);
    writer.WriteArithmetic(eVMOperationGt);
    writer.WriteArithmetic(eVMOperationLt);
    writer.WriteArithmetic(eVMOperationAnd);
    writer.WriteArithmetic(eVMOperationOr);
    writer.WriteArithmetic(eVMOperationNot);

    writer.WriteGoto("gotoLabel");
    writer.WriteIfGoto("ifGotoLabel");
    writer.WriteLabel("labelName");

    writer.WritePop(eVMSegmentLocal, 2);
    writer.WritePop(eVMSegmentStatic, 2);
    writer.WritePop(eVMSegmentPointer, 2);
    writer.WritePop(eVMSegmentArg, 2);
    writer.WritePop(eVMSegmentThis, 2);
    writer.WritePop(eVMSegmentThat, 2);
    writer.WritePop(eVMSegmentTemp, 2);
    writer.WritePop(eVMSegmentConst, 2);

    writer.WritePush(eVMSegmentLocal, 2);
    writer.WritePush(eVMSegmentStatic, 2);
    writer.WritePush(eVMSegmentPointer, 2);
    writer.WritePush(eVMSegmentArg, 2);
    writer.WritePush(eVMSegmentThis, 2);
    writer.WritePush(eVMSegmentThat, 2);
    writer.WritePush(eVMSegmentTemp, 2);
    writer.WritePush(eVMSegmentConst, 2);

    writer.WriteReturn();
}

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
#endif // DEBUG_BUILD

int main(int argc, char * argv[])
{
    //return 0;

#ifndef DEBUG_BUILD
    if (argc != 2)
    {
        cerr << "usage: " << argv[0] << " <input file>.jack" << endl;
        return 1;
    }
#endif // DEBUG_BUILD

#ifdef DEBUG_BUILD
    TestSymbolTable();
    //TestVMWriter();
#endif // DEBUG_BUILD

#ifndef DEBUG_BUILD
    JackController jackController;
    string filename(argv[1]);
    size_t dotIndex = filename.find_last_of(".");

    if (dotIndex != string::npos)
    {
        jackController.CompileFile(filename);
    }
    else
    {
        jackController.CompileDirectory(filename);
    }
#endif // DEBUG_BUILD

    return 0;
}
