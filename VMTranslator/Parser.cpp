#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

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
    { "not", eCommandTypeArithmetic }
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

/* PUBLIC METHODS */
bool Parser::HasMoreCommands()
{
    return this->inputSource.good();
}

bool IsValidSegment(const string& segment)
{
    return validSegments.find(segment) != validSegments.end();
}

void Parser::SetDefaultOutputs()
{
    this->commandType = eCommandTypeEmpty;
    this->argument1 = "";
    this->argument2 = 0;
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

            // handle invariants

            // index must be non-negative
            if (index < 0)
            {
                this->SetDefaultOutputs();
                throw runtime_error(NON_NEGATIVE_INDEX_ERR + line);
            }

            // When accessing pointer segment, only valied indices are 0 and 1
            if ((segment == "pointer") && (index != 0) && (index != 1))
            {
                this->SetDefaultOutputs();
                throw runtime_error(SYNTAX_ERR_MSG + line);
            }

            // temp section contains only 8 entries
            if (segment == "temp" && index >= 8)
            {
                this->SetDefaultOutputs();
                throw runtime_error(TEMP_ERR_MSG + line);
            }

            // cannnot pop from constant section
            if (this->commandType == eCommandTypePop && segment == "constant")
            {
                this->SetDefaultOutputs();
                throw runtime_error(POP_CONST_ERR_MSG + line);
            }

            this->argument1 = segment;
            this->argument2 = index;
        }
        else if (this->commandType == eCommandTypeArithmetic)
        {
            // set the command itself as argument 1 on Arithmetic commands
            this->argument1 = command;
            this->argument2 = 0;
        }
        // TODO: Extend API for other commands
    }
    else
    {
        this->SetDefaultOutputs();
        throw runtime_error(SYNTAX_ERR_MSG + line);
    }
}
