#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <dirent.h>

#include "VMTranslator.h"
#include "Parser.h"

using namespace std;
using namespace HackVMTranslator;

bool IsValidVmFile(const string& filename)
{
    int dotIndex = filename.find_last_of(".");
    return (filename.substr(dotIndex + 1) == "vm");
}

/* PRIVATE METHODS */
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
    case eCommandTypeLabel:
    case eCommandTypeGoto:
    case eCommandTypeIf:
        codeWriter.WriteBranchCommand(command.commandType, command.argument1);
        break;
    case eCommandTypeCall:
    case eCommandTypeFunction:
        codeWriter.WriteFunctionCommand(command.commandType, command.argument1, command.argument2);
        break;
    case eCommandTypeReturn:
        codeWriter.WriteReturnCommand();
        break;
    default:
        // Command should always be recognized by this stage
        assert(false);
    }
}

void VMTranslator::TranslateFileInternal(const string& inputFilename)
{
    ifstream istr(inputFilename);
    string line;
    Command currentCommand;

    if (!istr)
    {
        cerr << "Cannot open input file: " << inputFilename << endl;
        return;
    }

    Parser parser(istr);
    while (parser.HasMoreCommands())
    {
        try
        {
            parser.Advance();
        }
        catch (runtime_error const& e)
        {
            codeWriter.CloseStream();
            std::remove(codeWriter.GetOutputFilename().c_str());
            cerr << e.what();
            return;
        }

        currentCommand.commandType = parser.GetCommandType();
        currentCommand.argument1 = parser.GetArgument1();
        currentCommand.argument2 = parser.GetArgument2();

        this->TranslateToAssembly(codeWriter, currentCommand);
    }
}

/* PUBLIC METHODS */
void VMTranslator::TranslateDirectory(const string& directoryName)
{
    struct dirent *entry;
    DIR *dp;
    string currentFilename;

    dp = opendir(directoryName.c_str());
    if (dp == NULL) {
        cerr << ("Specified Path does not exist or could not be read.");
        return;
    }

    codeWriter.SetOutputFilename(directoryName + ".asm");
    codeWriter.WriteStartupCode();

    while ((entry = readdir(dp)))
    {
        currentFilename = entry->d_name;
        if (IsValidVmFile(currentFilename))
        {
            // remove extension of file
            int dotIndex = currentFilename.find_last_of(".");
            string processedFile;
            if (dotIndex != string::npos)
            {
                processedFile = currentFilename.substr(0, dotIndex);
            }
            else
            {
                processedFile = currentFilename;
            }

            codeWriter.SetProcessedFilename(processedFile);
            this->TranslateFileInternal(directoryName + "/" + currentFilename);
        }
    }

    closedir(dp);
}

void VMTranslator::TranslateFile(const string& inputFilename)
{
    int dotIndex = inputFilename.find_last_of(".");
    string outputFilename = inputFilename.substr(0, dotIndex);
    string outputFilenameWithExtension = outputFilename + ".asm";

    if (!IsValidVmFile(inputFilename))
    {
        cerr << "Invalid file extension: " << inputFilename << endl;
        return;
    }

    codeWriter.SetOutputFilename(outputFilenameWithExtension);
    codeWriter.SetProcessedFilename(outputFilename);

    this->TranslateFileInternal(inputFilename);
}
