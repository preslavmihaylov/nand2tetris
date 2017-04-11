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
        void Translate(const std::string& filename);
    private:
        void TranslateToAssembly(CodeWriter& codeWriter, const Command& command);
    };
}

#endif // VMTRANSLATOR_H
