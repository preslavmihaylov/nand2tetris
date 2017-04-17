#ifndef COMPILATION_ENGINE_H
#define COMPILATION_ENGINE_H

#include "JackTokenizer.h"

namespace JackCompiler
{
    class CompilationEngine
    {
    public:
        CompilationEngine(const std::string& inputFilename, const std::string& outputFilename)
            : tokenizer(inputFilename), outputStream(outputFilename) {};

    void CompileFile();
    private:
        JackTokenizer tokenizer;
        std::ofstream outputStream;

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
        void ExpectIdentifier();
        void ExpectIntConst();
        void ExpectStringConst();

        bool IsNextTokenTerm();
        bool IsNextTokenOperation();
    };
}

#endif // COMPILATION_ENGINE_H
