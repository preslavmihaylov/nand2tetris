#ifndef COMPILATION_ENGINE_H
#define COMPILATION_ENGINE_H

#include "JackTokenizer.h"
#include "SymbolTable.h"

namespace JackCompiler
{
    enum eIdentifierType
    {
        eIdentifierTypeDeclaration,
        eIdentifierTypeUsage,
        eIdentifierTypeCount,
        eIdentifierTypeNone
    };

    class CompilationEngine
    {
    public:
        CompilationEngine(const std::string& inputFilename, const std::string& outputFilename)
            : tokenizer(inputFilename), outputStream(outputFilename) {};

    void CompileFile();
    private:
        JackTokenizer tokenizer;
        std::ofstream outputStream;
        SymbolTable symbolTable;

        void CompileClass();
        bool CompileClassVarDec();
        bool CompileSubroutine();
        void CompileParameterList();
        bool CompileVarDec();
        void CompileStatements();
        bool CompileDo();
        bool CompileLet();
        bool CompileWhile();
        bool CompileReturn();
        bool CompileIf();
        void CompileExpressionList();
        bool CompileExpression();
        void CompileTerm();

        void ExpectKeyword(eKeyword keyword);
        void ExpectSymbol(char symbol);
        void ExpectIdentifier(eIdentifierType expectedIdType,
                              std::string idType = std::string(),
                              eVariableKind idKind = eVariableKindNone);
        void ExpectIntConst();
        void ExpectStringConst();

        bool IsNextTokenTerm();
        bool IsNextTokenOperation();
        std::string GetIdentifierXMLFormat(const std::string& id);
    };
}

#endif // COMPILATION_ENGINE_H
