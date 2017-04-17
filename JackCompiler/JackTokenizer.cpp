#include <cassert>
#include <unordered_set>
#include <iostream>
#include <sstream>

#include "JackTokenizer.h"

using namespace std;
using namespace JackCompiler;

#define INVALID_SYMBOL_ERR ("Invalid symbol located at line ")
#define LARGE_CONST_ERR ("Invalid constant located at line ")
#define NEWLINE_CHAR ('\n')
#define ASTERISK_CHAR ('*')
#define FORWARD_SLASH_CHAR ('/')
#define ESCAPE_CHARACTER ('\\')
#define JACK_INT_MAX (32768)

unordered_set<char> validSymbols =
{
    '{',
    '}',
    '(',
    ')',
    '[',
    ']',
    '.',
    ',',
    ';',
    '+',
    '-',
    '*',
    '/',
    '&',
    '|',
    '<',
    '>',
    '=',
    '~'
};

unordered_set<string> validKeywords =
{
    "class",
    "constructor",
    "function",
    "method",
    "field",
    "static",
    "var",
    "int",
    "char",
    "boolean",
    "void",
    "true",
    "false",
    "null",
    "this",
    "let",
    "do",
    "if",
    "else",
    "while",
    "return"
};

/* UTILITY METHODS */
void JackTokenizer::ThrowException(const string& exMsg)
{
    ostringstream oss;
    oss << exMsg << this->lineNumber;
    throw runtime_error(oss.str());
}

bool IsCommentSymbol(char symbol)
{
    return (symbol == '/');
}

bool IsValidKeyword(const string& str)
{
    return (validKeywords.find(str) != validKeywords.end());
}

bool IsValidSymbol(char symbol)
{
    return (validSymbols.find(symbol) != validSymbols.end());
}

bool IsDoubleQuote(char symbol)
{
    return (symbol == '\"');
}

bool IsDigit(char symbol)
{
    return (symbol >= '0' && symbol <= '9');
}

bool IsIdentifierBeginning(char symbol)
{
    return ((isalpha(symbol)) || (symbol == '_'));
}

bool IsIdentifierSymbol(char symbol)
{
    return IsIdentifierBeginning(symbol) || IsDigit(symbol);
}

bool IsEscapeCharacter(char symbol)
{
    return symbol == ESCAPE_CHARACTER;
}

/* PRIVATE METHODS */
void JackTokenizer::SetTokenType(eTokenType tokenType)
{
    this->tokenType = tokenType;
}

void JackTokenizer::SetKeyword(const string& strVal)
{
    this->SetDefaultOutputs();

    if (strVal == "class") this->keywordType = eKeywordClass;
    else if (strVal == "constructor") this->keywordType = eKeywordConstructor;
    else if (strVal == "function") this->keywordType = eKeywordFunction;
    else if (strVal == "method") this->keywordType = eKeywordMethod;
    else if (strVal == "field") this->keywordType = eKeywordField;
    else if (strVal == "static") this->keywordType = eKeywordStatic;
    else if (strVal == "var") this->keywordType = eKeywordVar;
    else if (strVal == "int") this->keywordType = eKeywordInt;
    else if (strVal == "char") this->keywordType = eKeywordChar;
    else if (strVal == "boolean") this->keywordType = eKeywordBoolean;
    else if (strVal == "void") this->keywordType = eKeywordVoid;
    else if (strVal == "true") this->keywordType = eKeywordTrue;
    else if (strVal == "false") this->keywordType = eKeywordFalse;
    else if (strVal == "null") this->keywordType = eKeywordNull;
    else if (strVal == "this") this->keywordType = eKeywordThis;
    else if (strVal == "let") this->keywordType = eKeywordLet;
    else if (strVal == "do") this->keywordType = eKeywordDo;
    else if (strVal == "if") this->keywordType = eKeywordIf;
    else if (strVal == "else") this->keywordType = eKeywordElse;
    else if (strVal == "while") this->keywordType = eKeywordWhile;
    else if (strVal == "return") this->keywordType = eKeywordReturn;
    else
    {
        // should always pass a valid keywordType
        assert(false);
    }

    this->keyword = strVal;
}

void JackTokenizer::SetSymbol(char symbol)
{
    this->SetDefaultOutputs();
    this->symbol = symbol;
}

void JackTokenizer::SetIdentifier(string identifier)
{
    this->SetDefaultOutputs();
    this->identifier = identifier;
}

void JackTokenizer::SetIntVal(int intVal)
{
    this->SetDefaultOutputs();
    this->intVal = intVal;
}

void JackTokenizer::setStringVal(string stringVal)
{
    this->SetDefaultOutputs();
    this->stringVal = stringVal;
}

void JackTokenizer::SetDefaultOutputs()
{
    this->keyword = eKeywordNone;
    this->symbol = 0;
    this->identifier = "";
    this->intVal = 0;
    this->stringVal = "";
}

/* PUBLIC METHODS */
bool JackTokenizer::HasMoreTokens()
{
    return !this->inputStream.eof();
}

eTokenType JackTokenizer::GetTokenType() const
{
    return this->tokenType;
}

eKeyword JackTokenizer::GetKeywordType() const
{
    if (this->tokenType != eTokenTypeKeyword)
    {
        // shouldn't be invoked otherwise
        assert(false);
        return eKeywordNone;
    }

    return this->keywordType;
}

string JackTokenizer::GetKeyword() const
{
    if (this->tokenType != eTokenTypeKeyword)
    {
        // shouldn't be invoked otherwise
        assert(false);
        return string();
    }

    return this->keyword;
}

char JackTokenizer::GetSymbol() const
{
    if (this->tokenType != eTokenTypeSymbol)
    {
        // shouldn't be invoked otherwise
        assert(false);
        return 0;
    }

    return this->symbol;
}

string JackTokenizer::GetIdentifier() const
{
    if (this->tokenType != eTokenTypeIdentifier)
    {
        // shouldn't be invoked otherwise
        assert(false);
        return string();
    }

    return this->identifier;
}

int JackTokenizer::GetIntVal() const
{
    if (this->tokenType != eTokenTypeIntConst)
    {
        // shouldn't be invoked otherwise
        assert(false);
        return 0;
    }

    return this->intVal;
}

string JackTokenizer::GetStringVal() const
{
    if (this->tokenType != eTokenTypeStringConst)
    {
        // shouldn't be invoked otherwise
        assert(false);
        return string();
    }

    return this->stringVal;
}

void JackTokenizer::GetNextSymbol(char& symbol)
{
    this->inputStream.get(symbol);

    if (symbol == NEWLINE_CHAR)
    {
        ++this->lineNumber;
    }
}

bool JackTokenizer::SkipComment()
{
    char currentSymbol;
    bool commentSkipped = true;

    if (!this->HasMoreTokens())
    {
        return commentSkipped;
    }

    this->GetNextSymbol(currentSymbol);

    if (IsCommentSymbol(currentSymbol))
    {
        // It is this type of comment

        // Stop when end of line is encountered
        while (currentSymbol != NEWLINE_CHAR)
        {
            this->GetNextSymbol(currentSymbol);
        }
    }
    else if (currentSymbol == ASTERISK_CHAR)
    {
        /* It is this type of comment */

        // Stop when "*/" is encountered
        while (currentSymbol != FORWARD_SLASH_CHAR)
        {
            do
            {
                this->GetNextSymbol(currentSymbol);
            } while (currentSymbol != ASTERISK_CHAR && this->HasMoreTokens());

            if (!this->HasMoreTokens())
            {
                // Invalid comment
                ThrowException(INVALID_SYMBOL_ERR);
            }

            this->GetNextSymbol(currentSymbol);
        }
    }
    else
    {
        // a divide symbol
        this->PutbackSymbol(currentSymbol);
        commentSkipped = false;
    }

    return commentSkipped;
}

void JackTokenizer::PutbackSymbol(char symbol)
{
    this->inputStream.putback(symbol);
}

void JackTokenizer::PutbackToken()
{
    this->isTokenPreserved = true;
}

void JackTokenizer::ExtractSymbol(char currentSymbol)
{
    this->SetTokenType(eTokenTypeSymbol);
    this->SetSymbol(currentSymbol);
}

void JackTokenizer::ExtractStringConst(char currentSymbol)
{
    string strVal;

    // ignore '\"'
    this->GetNextSymbol(currentSymbol);

    // extract int value until unknown symbol is encountered
    while (!IsDoubleQuote(currentSymbol) &&
           this->HasMoreTokens())
    {
        // escaped characters bypass validation
        if (IsEscapeCharacter(currentSymbol))
        {
            this->GetNextSymbol(currentSymbol);
        }

        strVal += currentSymbol;
        this->GetNextSymbol(currentSymbol);
    }

    this->SetTokenType(eTokenTypeStringConst);
    this->setStringVal(strVal);
}

void JackTokenizer::ExtractIntValue(char currentSymbol)
{
    unsigned long long val = 0;

    // extract int value until unknown symbol is encountered
    while (IsDigit(currentSymbol) && this->HasMoreTokens())
    {
        val *= 10;
        val += (currentSymbol - '0');
        this->GetNextSymbol(currentSymbol);
    }

    if (this->HasMoreTokens() && currentSymbol != NEWLINE_CHAR)
    {
        // return non-digit symbol to stream
        this->PutbackSymbol(currentSymbol);
    }
    if (val > JACK_INT_MAX)
    {
        ThrowException(LARGE_CONST_ERR);
    }

    this->SetTokenType(eTokenTypeIntConst);
    this->SetIntVal(val);
}

void JackTokenizer::ExtractIdentifier(char currentSymbol)
{
    string id;
    // extract identifier until unknown symbol is encountered
    while (IsIdentifierSymbol(currentSymbol) && this->HasMoreTokens())
    {
        id += currentSymbol;
        this->GetNextSymbol(currentSymbol);
    }

    if (this->HasMoreTokens() && currentSymbol != NEWLINE_CHAR)
    {
        // return non-digit symbol to stream
        this->PutbackSymbol(currentSymbol);
    }

    if (IsValidKeyword(id))
    {
        this->SetTokenType(eTokenTypeKeyword);
        this->SetKeyword(id);
    }
    else
    {
        this->SetTokenType(eTokenTypeIdentifier);
        this->SetIdentifier(id);
    }
}

void JackTokenizer::Advance()
{
    char currentSymbol;
    if (this->isTokenPreserved)
    {
        this->isTokenPreserved = false;
        return;
    }

    do {
        this->GetNextSymbol(currentSymbol);
        bool isCommentSkipped = true;

        // skip comment and get first symbol after comment
        while (isCommentSkipped && IsCommentSymbol(currentSymbol))
        {
            isCommentSkipped = this->SkipComment();
            // if not skipped, next character is a divide symbol
            if (isCommentSkipped)
            {
                this->GetNextSymbol(currentSymbol);
            }
        }
    // skip whitespace
    } while (isspace(currentSymbol) && this->HasMoreTokens());

    // no more tokens, don't do anything
    if (!this->HasMoreTokens())
    {
        this->tokenType = eTokenTypeNone;
        this->SetDefaultOutputs();
        return;
    }

    if (IsValidSymbol(currentSymbol))
    {
        this->ExtractSymbol(currentSymbol);
    }
    else if (IsDoubleQuote(currentSymbol))
    {
        this->ExtractStringConst(currentSymbol);
    }
    else if (IsDigit(currentSymbol))
    {
        this->ExtractIntValue(currentSymbol);
    }
    else if (IsIdentifierBeginning(currentSymbol))
    {
        this->ExtractIdentifier(currentSymbol);
    }
    else
    {
        // invalid symbol
        ThrowException(INVALID_SYMBOL_ERR);
    }
}
