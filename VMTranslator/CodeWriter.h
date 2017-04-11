#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <fstream>
#include <string>

#include "CommandType.h"

namespace HackVMTranslator
{
    class CodeWriter
    {
    public:
        CodeWriter(const std::string& outputFilename)
            : outputDest(outputFilename), outputFilename(outputFilename), labelsCounter(0) {};

        void WriteArithmetic(const std::string& command);
        void WritePushPop(eCommandType commandType, const std::string& segment, int index);
        void CloseStream() { this->outputDest.close(); }
    private:
        std::ofstream outputDest;
        std::string outputFilename;
        size_t labelsCounter;

        void CompileAddCommand();
        void CompileSubtractCommand();
        void CompileNegateCommand();
        void CompileEqualsCommand();
        void CompileGreaterThanCommand();
        void CompileLessThanCommand();
        void CompileAndCommand();
        void CompileOrCommand();
        void CompileNotCommand();

        void CompileSimplePushCommand(const std::string& segmentLabel, int index);
        void CompileConstantPushCommand(int index);
        void CompileStaticPushCommand(int index);
        void CompilePointerPushCommand(int index);

        void CompileSimplePopCommand(const std::string& segment, int index);
        void CompileStaticPopCommand(int index);
        void CompilePointerPopCommand(int index);
    };
}

#endif // CODEWRITER_H
