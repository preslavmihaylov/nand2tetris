#ifndef COMPILATION_ENGINE_H
#define COMPILATION_ENGINE_H

#include "JackTokenizer.h"
#include "SymbolTable.h"
#include "VMWriter.h"

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
            : tokenizer(inputFilename),
              vmWriter(outputFilename),
              autoIncrementCounter(0)
        {
            size_t indexOfSlash = outputFilename.find_last_of("/");
            size_t indexOfDot = outputFilename.find_last_of(".");
            this->outputFilename = outputFilename.substr(indexOfSlash + 1, (indexOfDot - indexOfSlash - 1));
        };

    void CompileFile();
    private:
        JackTokenizer tokenizer;
        SymbolTable symbolTable;
        VMWriter vmWriter;

        std::string outputFilename;
        size_t autoIncrementCounter;

        void CompileClass();
        bool CompileClassVarDec();
        bool CompileSubroutine();
        int CompileParameterList();
        int CompileVarDec();
        void CompileStatements();
        bool CompileDo();
        bool CompileLet();
        bool CompileWhile();
        bool CompileReturn();
        bool CompileIf();
        int CompileExpressionList();
        bool CompileExpression();
        void CompileTerm();

        void ExpectKeyword(eKeyword keyword);
        void ExpectSymbol(char symbol);
        std::string ExpectIdentifier(eIdentifierType expectedIdType,
                                     std::string idType = std::string(),
                                     eVariableKind idKind = eVariableKindNone);
        void ExpectIntConst();
        void ExpectStringConst();

        bool IsNextTokenTerm();
        bool IsNextTokenOperation();
        void WriteOperation(char operationSymbol, bool isUnary);
        std::string GenerateLabel();
        std::string GetFullSubroutineName(std::string subroutineName);
    };
}

#endif // COMPILATION_ENGINE_H
