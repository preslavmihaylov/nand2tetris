#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cassert>

#include "VMTranslator.h"
#include "Parser.h"

using namespace std;
using namespace HackVMTranslator;

void VMTranslator::TranslateToAssembly(CodeWriter& codeWriter, const Command& command)
{
    switch (command.commandType)
    {
    case eCommandTypeEmpty:
        break;
    case eCommandTypeArithmetic:
        codeWriter.WriteArithmetic(command.argument1);
        break;
    case eCommandTypePush:
    case eCommandTypePop:
        codeWriter.WritePushPop(command.commandType, command.argument1, command.argument2);
        break;
    default:
        // Command should always be recognized by this stage
        assert(false);
    }
}

void VMTranslator::Translate(const string& filename)
{
    ifstream istr(filename);
    string line;
    Command currentCommand;

    int dotIndex = filename.find_last_of(".");
    string outputFilename = filename.substr(0, dotIndex) + ".asm";

    if (filename.substr(dotIndex + 1) != "vm")
    {
        cout << "Invalid file extension: " << filename << endl;
        return;
    }
    else if (!istr)
    {
        cout << "Cannot open input file: " << filename << endl;
        return;
    }

    Parser parser(istr);
    CodeWriter codeWriter(outputFilename);

    while (parser.HasMoreCommands())
    {
        try
        {
            parser.Advance();
            /*
            cout << "Current Command: " << parser.CommandType() << endl;
            cout << "ARG1: " << parser.GetArgument1() << endl;
            cout << "ARG2: " << parser.GetArgument2() << endl;
            */


        }
        catch (runtime_error const& e)
        {
            codeWriter.CloseStream();
            std::remove(outputFilename.c_str());
            cerr << e.what();
            return;
        }

        currentCommand.commandType = parser.GetCommandType();
        currentCommand.argument1 = parser.GetArgument1();
        currentCommand.argument2 = parser.GetArgument2();

        this->TranslateToAssembly(codeWriter, currentCommand);
    }
}
