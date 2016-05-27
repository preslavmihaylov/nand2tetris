#include "parser.h"
#include "symbol-table.h"

#include <stack>

#define MAX_STR_LNGTH 255

using namespace std;

static char buffer[MAX_STR_LNGTH];

/* Utility functions */
bool IsValidCharacter(char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9');
}

bool IsAtSymbol(char ch)
{
    return ch == '@';
}

bool IsWhitespace(char ch)
{
    return ch == ' ' || ch == '\t';
}

bool IsBackslash(char ch)
{
    return ch == '/';
}

void SkipWhitespace(string line, int * cnt)
{
    while (line[*cnt] != '\0' && (line[*cnt] == ' ' || line[*cnt] == '\t'))
    {
        (*cnt)++;
    }
}

/* Constructor/Destructor */
Parser::Parser(string filename)
{
    this->file.open(filename);
}

Parser::~Parser()
{
    this->file.close();
}

/* Public methods */
void Parser::PreprocessLabels()
{
    stack<string> labelsStack;
    bool isValid = false;

    while (this->HasNextInstruction())
    {
        this->GetNextInstruction();
        isValid = this->isAInstruction || this->isCInstruction;

        if (this->isLabel)
        {
            labelsStack.push(this->label);
        }
        else if (isValid)
        {
            ++this->lineNumber;
            while (!labelsStack.empty())
            {
                string label = labelsStack.top();
                labelsStack.pop();

                SYMBOL_TABLE.AddSymbol(label, this->lineNumber);
            }
        }
    }

    // Reset input stream to start of file
    this->file.clear();
    this->file.seekg(0, ios::beg);
}

void Parser::GetNextValidInstruction()
{
    bool isValid = false;
    while (!isValid && this->HasNextInstruction())
    {
        this->GetNextInstruction();
        isValid = this->isAInstruction || this->isCInstruction;
    }

    if (isValid)
    {
        ++this->lineNumber;
    }
}

bool Parser::HasNextInstruction()
{
    return !this->file.eof();
}

int Parser::GetLineNumber()
{
    return this->lineNumber;
}

string Parser::GetAValue()
{
    return this->value;
}

string Parser::GetDest()
{
    return this->dest;
}
string Parser::GetComp()
{
    return this->comp;
}

string Parser::GetJump()
{
    return this->jump;
}

bool Parser::IsAInstruction()
{
    return this->isAInstruction;
}

bool Parser::IsCInstruction()
{
    return this->isCInstruction;
}

/* Private Methods */
void Parser::GetNextInstruction()
{
    string line;
    getline(this->file, line);

    this->isAInstruction = this->ExtractAInstruction(line);
    this->isCInstruction = this->ExtractCInstruction(line);
    this->isLabel = this->ExtractLabel(line);
}

bool Parser::ExtractLabel(string line)
{
    int buff_index = 0;
    int cnt = 0;

    // Ignore whitespace in the start of the instruction
    SkipWhitespace(line, &cnt);

    if (line[cnt] == '(')
    {
        ++cnt;
        SkipWhitespace(line, &cnt);

        while (line[cnt] != ')' &&
               line[cnt] != '\0' &&
               !IsBackslash(line[cnt]) &&
               !IsWhitespace(line[cnt]))
        {
            buffer[buff_index++] = line[cnt++];
        }

        SkipWhitespace(line, &cnt);
    }

    if (buff_index > 0 && line[cnt] == ')')
    {
        buffer[buff_index] = '\0';
        this->label = buffer;
        return true;
    }

    return false;
}

bool Parser::ExtractCInstruction(string line)
{
    int cnt = 0;
    bool hasReadDest = false;
    bool hasReadComp = false;
    bool hasReadJump = false;

    hasReadDest = this->ReadDest(line, &cnt);
    hasReadComp = this->ReadComp(line, &cnt);
    hasReadJump = this->ReadJump(line, &cnt);

    return hasReadDest && hasReadComp && hasReadJump;
}

bool Parser::ExtractAInstruction(string line)
{
    int cnt = 0;
    int buff_index = 0;

    SkipWhitespace(line, &cnt);

    if (line[cnt] == '@')
    {
        cnt++;
        SkipWhitespace(line, &cnt);

        while (line[cnt] != '\0' &&
               !IsBackslash(line[cnt]) &&
               !IsWhitespace(line[cnt]) &&
               !IsAtSymbol(line[cnt]))
        {
            buffer[buff_index++] = line[cnt++];
        }
    }

    SkipWhitespace(line, &cnt);

    if (buff_index > 0 &&
        (line[cnt] == '\0' || IsBackslash(line[cnt])))
    {
        buffer[buff_index] = '\0';
        this->value = buffer;
        return true;
    }

    return false;
}

bool Parser::ReadDest(string line, int * cnt)
{
    int buff_index = 0;

    // Ignore whitespace in the start of the instruction
    SkipWhitespace(line, cnt);

    // Read Dest part of the instruction
    while (line[*cnt] != '=' &&
           line[*cnt] != '\0' &&
           !IsBackslash(line[*cnt]) &&
           !IsWhitespace(line[*cnt]) &&
           !IsAtSymbol(line[*cnt]) &&
           line[*cnt] != '(' &&
           IsValidCharacter(line[*cnt]))
    {
        buffer[buff_index++] = line[(*cnt)++];
    }

    // Ignore whitespace in the end of the instruction
    SkipWhitespace(line, cnt);

    buffer[buff_index] = '\0';

    // If instruction is X=... , then it is valid
    if (line[*cnt] == '=' && buff_index > 0)
    {
        this->dest = buffer;

        // Ignore '=' character
        (*cnt)++;
        return true;
    }
    else if (line[*cnt] != '=' &&
             !IsAtSymbol(line[*cnt]) &&
             // Should not be a label
             line[*cnt] != '(')
    {
        buff_index = 0;
        buffer[buff_index++] = 'n';
        buffer[buff_index++] = 'u';
        buffer[buff_index++] = 'l';
        buffer[buff_index++] = 'l';
        buffer[buff_index] = '\0';

        this->dest = buffer;
        (*cnt) = 0;
        return true;
    }

    // Encountered '=' but no instruction was read
    return false;
}

bool Parser::ReadComp(string line, int * cnt)
{
    int buff_index = 0;

    // Ignore whitespace in the start of the instruction
    SkipWhitespace(line, cnt);

    while (line[*cnt] != ';' &&
           line[*cnt] != '\0' &&
           !IsBackslash(line[*cnt]) &&
           !IsWhitespace(line[*cnt]))
    {
        buffer[buff_index++] = line[(*cnt)++];
    }

    // Ignore whitespace in the end of the instruction
    SkipWhitespace(line, cnt);

    if (buff_index > 0)
    {
        buffer[buff_index] = '\0';
        this->comp = buffer;
        return true;
    }

    return false;
}

bool Parser::ReadJump(string line, int * cnt)
{
    int buff_index = 0;
    bool hasReadSemicolon = false;

    // Only if semicolon is met, an explicit JUMP instruction is specified
    if (line[*cnt] == ';')
    {
        hasReadSemicolon = true;
        (*cnt)++;
    }

    SkipWhitespace(line, cnt);

    while (line[*cnt] != '\0' &&
           line[*cnt] != '/' &&
           !IsWhitespace(line[*cnt]))
    {
        buffer[buff_index++] = line[(*cnt)++];
    }

    if (hasReadSemicolon == true && buff_index > 0)
    {
        buffer[buff_index] = '\0';
        this->jump = buffer;
        return true;
    }
    else if (hasReadSemicolon != true && buff_index == 0)
    {
        // If no explicit jump instruction --> it is by default null
        buffer[buff_index++] = 'n';
        buffer[buff_index++] = 'u';
        buffer[buff_index++] = 'l';
        buffer[buff_index++] = 'l';
        buffer[buff_index] = '\0';

        this->jump = buffer;
        return true;
    }

    // No semicolon read && text is present  --> Invalid
    // semicolon read && text is not present --> Invalid
    return false;
}
