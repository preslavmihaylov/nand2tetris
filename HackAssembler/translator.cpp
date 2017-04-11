#include "translator.h"
#include "symbol-table.h"

#include <map>
#include <stdlib.h>


using namespace std;

static map<string, string> destMap =
{
    {"null", "000"},
    {"M",    "001"},
    {"D",    "010"},
    {"MD",   "011"},
    {"A",    "100"},
    {"AM",   "101"},
    {"AD",   "110"},
    {"AMD",  "111"},
};

static map<string, string> compMap =
{
    {"0",    "0101010"},
    {"1",    "0111111"},
    {"-1",   "0111010"},
    {"D",    "0001100"},
    {"A",    "0110000"},
    {"!D",   "0001101"},
    {"!A",   "0110001"},
    {"-D",   "0001111"},
    {"-A",   "0110011"},
    {"D+1",  "0011111"},
    {"A+1",  "0110111"},
    {"D-1",  "0001110"},
    {"A-1",  "0110010"},
    {"D+A",  "0000010"},
    {"D-A",  "0010011"},
    {"A-D",  "0000111"},
    {"D&A",  "0000000"},
    {"D|A",  "0010101"},
    {"M",    "1110000"},
    {"!M",   "1110001"},
    {"-M",   "1110011"},
    {"M+1",  "1110111"},
    {"M-1",  "1110010"},
    {"D+M",  "1000010"},
    {"D-M",  "1010011"},
    {"M-D",  "1000111"},
    {"D&M",  "1000000"},
    {"D|M",  "1010101"},
};

static map<string, string> jumpMap =
{
    {"null", "000"},
    {"JGT",  "001"},
    {"JEQ",  "010"},
    {"JGE",  "011"},
    {"JLT",  "100"},
    {"JNE",  "101"},
    {"JLE",  "110"},
    {"JMP",  "111"},
};

#define MAX_INSTR_LENGTH 16

static string cOpcode = "111";
static string aOpcode = "0";
static char aInstrBuffer[MAX_INSTR_LENGTH];

bool IsNumber(string input)
{
    for (int cnt = 0; input[cnt] != '\0'; ++cnt)
    {
        if (input[cnt] < '0' || input[cnt] > '9')
        {
            return false;
        }
    }

    return true;
}

string Translator::TranslateAInstruction(string value)
{
    string result;
    int number = -1;

    if (IsNumber(value))
    {
        number = atoi(value.c_str());
    }
    else
    {
        if (!SYMBOL_TABLE.SymbolExists(value))
        {
            SYMBOL_TABLE.AddSymbol(value);
        }

        number = SYMBOL_TABLE.GetSymbol(value);
    }

    aInstrBuffer[0] = 'a';
    int buffIndex = MAX_INSTR_LENGTH - 1;
    aInstrBuffer[buffIndex--] = '\0';

    if (number != -1)
    {
        // convert to binary
        while (number > 0)
        {
            aInstrBuffer[buffIndex--] = (number % 2) + '0';
            number /= 2;
        }

        // Fill the rest of the buffer with zeroes
        while (buffIndex >= 0)
        {
            aInstrBuffer[buffIndex--] = '0';
        }

        result = aOpcode + aInstrBuffer;
    }

    return result;
}

string Translator::TranslateCInstruction(string dest, string comp, string jump)
{
    string destInstr;
    string compInstr;
    string jumpInstr;
    string result;

    // Does dest key exist
    if (destMap.find(dest) != destMap.end())
    {
        destInstr = destMap[dest];
    }

    // Does comp key exist
    if (compMap.find(comp) != compMap.end())
    {
        compInstr = compMap[comp];
    }

    // Does jump key exist
    if (jumpMap.find(jump) != jumpMap.end())
    {
        jumpInstr = jumpMap[jump];
    }

    if (!destInstr.empty() && !compInstr.empty() && !jumpInstr.empty())
    {
        result = cOpcode + compInstr + destInstr + jumpInstr;
    }

    return result;
}
