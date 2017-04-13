#ifndef VMTRANSLATOR_H
#define VMTRANSLATOR_H

#include <string>

#include "CodeWriter.h"

namespace HackVMTranslator
{
    struct Command
    {
        eCommandType commandType;
        std::string argument1;
        int argument2;
    };

    class VMTranslator
    {
    public:
        void TranslateDirectory(const std::string& directoryName);
        void TranslateFile(const std::string& inputFilename);
    private:
        CodeWriter codeWriter;

        void TranslateToAssembly(CodeWriter& codeWriter, const Command& command);
        void TranslateFileInternal(const std::string& inputFilename);
    };
}

#endif // VMTRANSLATOR_H
