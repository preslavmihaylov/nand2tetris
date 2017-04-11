#ifndef PARSER_H
#define PARSER_H

#include <fstream>

using namespace std;

class Parser
{
    public:
        Parser(string filename);
        ~Parser();
        void PreprocessLabels();
        bool HasNextInstruction();
        void GetNextValidInstruction();
        bool IsAInstruction();
        bool IsCInstruction();
        int GetLineNumber();
        string GetAValue();
        string GetDest();
        string GetComp();
        string GetJump();
    private:
        ifstream file;
        string value;
        string dest;
        string comp;
        string jump;
        string label;
        int lineNumber = -1;
        bool isAInstruction;
        bool isCInstruction;
        bool isLabel;
        void GetNextInstruction();
        bool ExtractAInstruction(string line);
        bool ExtractCInstruction(string line);
        bool ExtractLabel(string line);
        bool ReadDest(string line, int * cnt);
        bool ReadComp(string line, int * cnt);
        bool ReadJump(string line, int * cnt);
};

#endif // PARSER_H
