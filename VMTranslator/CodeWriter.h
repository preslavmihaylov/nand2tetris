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
        CodeWriter()
            : labelsCounter(0) {};

        CodeWriter(const std::string& outputFilename)
            : outputDest(outputFilename),
              outputFilename(outputFilename),
              processedFilename(outputFilename),
              labelsCounter(0) {};

        CodeWriter(const std::string& outputFilename, const std::string& processedFilename)
            : outputDest(outputFilename),
              outputFilename(outputFilename),
              processedFilename(processedFilename),
              labelsCounter(0) {};

        void WriteArithmetic(const std::string& command);
        void WritePushPop(eCommandType commandType, const std::string& segment, int index);
        void WriteBranchCommand(eCommandType commandType, const std::string& label);
        void WriteFunctionCommand(eCommandType commandType, const std::string& funcName, int argumentsCnt);
        void WriteReturnCommand();

        void WriteStartupCode();
        void CloseStream() { this->outputDest.close(); }
        void SetOutputFilename(const std::string& outputFilename)
        {
            this->outputFilename = outputFilename;
            this->outputDest.open(outputFilename);
        }

        void SetProcessedFilename(const std::string& filename) { this->processedFilename = filename; }

        std::string GetOutputFilename() const { return this->outputFilename; }
    private:
        std::ofstream outputDest;
        std::string outputFilename;
        std::string processedFilename;
        std::string processedFunction;
        size_t labelsCounter;

        // arithmetic compilation
        void CompileSumationCommand(const std::string& command);
        void CompileNegateCommand();
        void CompileComparisonCommand(const std::string& command);
        void CompileLogicalCommand(const std::string& command);
        void CompileNotCommand();

        // push compilation
        void CompileSimplePushCommand(const std::string& segmentLabel, int index);
        void CompileConstantPushCommand(int index);
        void CompileStaticPushCommand(int index);
        void CompilePointerPushCommand(int index);

        // pop compilation
        void CompileSimplePopCommand(const std::string& segment, int index);
        void CompileStaticPopCommand(int index);
        void CompilePointerPopCommand(int index);

        // branching compilation
        void CompileLabelCommand(const std::string& label);
        void CompileGotoCommand(const std::string& label);
        void CompileIfCommand(const std::string& label);

        // function commands compilation
        void CompileCallCommand(const std::string& functionName, int argsCnt);
        void CompileFunctionCommand(const std::string& functionName, int localsCnt);
        void CompileReturnCommand();
    };
}

#endif // CODEWRITER_H
