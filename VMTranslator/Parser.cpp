#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <cassert>

#include "Parser.h"

using namespace std;
using namespace HackVMTranslator;

#define COMMENT_CHARACTER ('/')
#define SYNTAX_ERR_MSG ("Syntax error encountered at: ")
#define TEMP_ERR_MSG ("Temp segment index cannot be greater than 7: ")
#define NON_NEGATIVE_INDEX_ERR ("Index must be a non-negative value: ")
#define POP_CONST_ERR_MSG ("Cannot pop into constant section: ")

/* PRIVATE STATIC DATA */

// string commands mapping to CommandType enumeration
static unordered_map<string, eCommandType> commandsMapping =
{
    { "push", eCommandTypePush },
    { "pop", eCommandTypePop },
    { "add", eCommandTypeArithmetic },
    { "sub", eCommandTypeArithmetic },
    { "neg", eCommandTypeArithmetic },
    { "eq", eCommandTypeArithmetic },
    { "gt", eCommandTypeArithmetic },
    { "lt", eCommandTypeArithmetic },
    { "and", eCommandTypeArithmetic },
    { "or", eCommandTypeArithmetic },
    { "not", eCommandTypeArithmetic },
    { "label", eCommandTypeLabel },
    { "goto", eCommandTypeGoto },
    { "if-goto", eCommandTypeIf },
    { "call", eCommandTypeCall },
    { "function", eCommandTypeFunction },
    { "return", eCommandTypeReturn }
};

static unordered_set<string> validSegments =
{
    "static",
    "local",
    "argument",
    "constant",
    "this",
    "that",
    "pointer",
    "temp"
};

/* PRIVATE FUNCTIONS */
bool IsComment(const string& str)
{
    if (str.size() < 2)
    {
        return false;
    }

    return (str[0] == str[1]) && (str[0] == COMMENT_CHARACTER);
}

bool IsEmpty(const string& str)
{
    for (int i = 0; i < str.size(); ++i)
    {
        if (!isspace(str[i]))
        {
            return false;
        }
    }

    return true;
}

bool CommandExists(const string& str)
{
    return commandsMapping.find(str) != commandsMapping.end();
}

bool IsValidSegment(const string& segment)
{
    return validSegments.find(segment) != validSegments.end();
}

/* PRIVATE METHODS */
void Parser::ValidateOperation(const string& command, const string& segment, int index, const string& line)
{
    bool isValid = true;
    string errMsg;

    // index must be non-negative
    if (index < 0)
    {
        isValid = false;
        errMsg = NON_NEGATIVE_INDEX_ERR + line;
    }
    // When accessing pointer segment, only valied indices are 0 and 1
    else if ((segment == "pointer") && (index != 0) && (index != 1))
    {
        isValid = false;
        errMsg = SYNTAX_ERR_MSG + line;
    }
    // temp section contains only 8 entries
    else if (segment == "temp" && index >= 8)
    {
        isValid = false;
        errMsg = TEMP_ERR_MSG + line;
    }
    // cannnot pop from constant section
    else if (this->commandType == eCommandTypePop && segment == "constant")
    {
        isValid = false;
        errMsg = POP_CONST_ERR_MSG + line;
    }

    if (!isValid)
    {
        this->SetDefaultOutputs();
        throw runtime_error(errMsg);
    }
}

void Parser::SetDefaultOutputs()
{
    this->commandType = eCommandTypeEmpty;
    this->argument1 = "";
    this->argument2 = 0;
}

/* PUBLIC METHODS */
bool Parser::HasMoreCommands()
{
    return this->inputSource.good();
}

void Parser::Advance()
{
    string line;
    string command;
    istringstream iss;

    // Skip empty and comment lines
    do
    {
        if (!getline(this->inputSource, line))
        {
            this->SetDefaultOutputs();
            return;
        }

        /*
        // Debug Info
        cout << "Current line: ";
        cout << line << endl;
        */

        iss.clear();
        iss.str(line);
        iss >> command;
    } while (IsComment(command) || IsEmpty(command));

    // transform command to lowercase
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);
    if (CommandExists(command))
    {
        this->commandType = commandsMapping[command];

        // Push and Pop commands have 2 arguments
        if ((this->commandType == eCommandTypePush) || (this->commandType == eCommandTypePop))
        {
            string segment;
            int index;
            iss >> segment;

            // transform segment to lowercase
            std::transform(segment.begin(), segment.end(), segment.begin(), ::tolower);

            // input stream is expected to have an integer after the segment
            if (!(iss >> index) || !IsValidSegment(segment))
            {
                this->SetDefaultOutputs();
                throw runtime_error(SYNTAX_ERR_MSG + line);
            }

            this->ValidateOperation(command, segment, index, line);

            this->argument1 = segment;
            this->argument2 = index;
        }
        else if (this->commandType == eCommandTypeArithmetic)
        {
            // set the command itself as argument 1 on Arithmetic commands
            this->argument1 = command;
            this->argument2 = 0;
        }
        else if ((this->commandType == eCommandTypeLabel) ||
                 (this->commandType == eCommandTypeGoto) ||
                 (this->commandType == eCommandTypeIf))
        {
            string label;

            // branching commands expect a label after the command
            if (!(iss >> label))
            {
                this->SetDefaultOutputs();
                throw runtime_error(SYNTAX_ERR_MSG + line);
            }

            this->argument1 = label;
        }
        else if ((this->commandType == eCommandTypeCall) ||
                 (this->commandType == eCommandTypeFunction))
        {
            string funcName;
            int argumentsCnt;
            iss >> funcName;

            // function commands expect a function name and arguments count after the command
            if (!(iss >> argumentsCnt))
            {
                this->SetDefaultOutputs();
                throw runtime_error(SYNTAX_ERR_MSG + line);
            }

            this->argument1 = funcName;
            this->argument2 = argumentsCnt;
        }
        else if (this->commandType == eCommandTypeReturn)
        {
            // nothing. Return command has no arguments
        }
        else
        {
            // command types should be valid by this point
            this->SetDefaultOutputs();
            assert(false);
            return;
        }

        // line should either be empty or end with a comment after an operation is read
        if (iss)
        {
            string leftover;
            iss >> leftover;

            if (leftover.size() != 0 && !IsComment(leftover))
            {
                this->SetDefaultOutputs();
                throw runtime_error(SYNTAX_ERR_MSG + line);
            }
        }
    }
    else
    {
        this->SetDefaultOutputs();
        throw runtime_error(SYNTAX_ERR_MSG + line);
    }
}
