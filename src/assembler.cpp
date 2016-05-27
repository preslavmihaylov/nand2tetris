#include "parser.h"
#include "translator.h"
#include "symbol-table.h"

#include <iostream>
#include <fstream>

#define MAX_STR_LNGTH 30

using namespace std;

char outFilename[MAX_STR_LNGTH];

bool FileExists(const char * filename)
{
    ifstream infile(filename);
    return infile.good();
}

void GenerateOutputFilename(const char * filename)
{
    int buff_index = 0;
    for (int cnt = 0; filename[cnt] != '\0' && filename[cnt] != '.'; ++cnt)
    {
        outFilename[buff_index++] = filename[cnt];
    }

    // Set extension
    outFilename[buff_index++] = '.';
    outFilename[buff_index++] = 'h';
    outFilename[buff_index++] = 'a';
    outFilename[buff_index++] = 'c';
    outFilename[buff_index++] = 'k';
    outFilename[buff_index] = '\0';
}

void Assemble(char * filename)
{
    Parser parser(filename);
    Translator translator;

    string result;
    string aValue;
    string dest;
    string comp;
    string jump;

    // Create/Overwrite output file
    GenerateOutputFilename(filename);
    ofstream outfile(outFilename);

    // Initial label preprocessing
    parser.PreprocessLabels();

    while (parser.HasNextInstruction())
    {
        parser.GetNextValidInstruction();

        // A-instruction processing
        if (parser.IsAInstruction())
        {
            aValue = parser.GetAValue();

            result = translator.TranslateAInstruction(aValue);

            if (!result.empty())
            {
                outfile << result << endl;
            }
            else
            {
                cout << "Invalid A Instr: "
                     << aValue
                     << " on line " << parser.GetLineNumber() << endl;

                break;
            }
        }
        // C-instruction processing
        else if (parser.IsCInstruction())
        {
            dest = parser.GetDest();
            comp = parser.GetComp();
            jump = parser.GetJump();

            result = translator.TranslateCInstruction(dest, comp, jump);

            if (!result.empty())
            {
                outfile << result << endl;
            }
            else
            {
                cout << "Invalid C Instr: "
                     << dest << "=" << comp << ";" << jump
                     << " on line " << parser.GetLineNumber() << endl;

                break;
            }
        }
    }

    outfile.close();
}

bool ValidateInput(int argc, char * argv[])
{
    if (argc != 2)
    {
        cout << "Invalid input parameters. Usage: HackAssembler <file>" << endl;
        return false;
    }
    else if (!FileExists(argv[1]))
    {
        cout << "input file does not exist" << endl;
        return false;
    }

    return true;
}

int main(int argc, char * argv[])
{
    bool isValidInput = ValidateInput(argc, argv);

    if (isValidInput)
    {
        Assemble(argv[1]);
    }
    else
    {
        return 1;
    }

    return 0;
}
