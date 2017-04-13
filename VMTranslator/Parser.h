#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <string>
#include "CommandType.h"

namespace HackVMTranslator
{
    class Parser
    {
    public:
        Parser(std::ifstream& ostr)
            : inputSource(ostr) {};

        bool HasMoreCommands();
        void Advance();
        HackVMTranslator::eCommandType GetCommandType() { return this->commandType; }
        std::string GetArgument1() { return this->argument1; };
        int GetArgument2() { return this->argument2; };
    private:
        std::ifstream& inputSource;
        HackVMTranslator::eCommandType commandType;
        std::string argument1;
        int argument2;

        void SetDefaultOutputs();
        void ValidateOperation(const std::string& command, const std::string& segment, int index, const std::string& line);
    };
}

#endif // PARSER_H
