#ifndef JACK_TOKENIZER_H
#define JACK_TOKENIZER_H

#include <string>
#include <fstream>

namespace JackCompiler
{
    enum eTokenType
    {
        eTokenTypeNone,
        eTokenTypeKeyword,
        eTokenTypeSymbol,
        eTokenTypeIdentifier,
        eTokenTypeIntConst,
        eTokenTypeStringConst,
        eTokenTypeCount
    };

    enum eKeyword
    {
        eKeywordNone,
        eKeywordClass,
        eKeywordMethod,
        eKeywordFunction,
        eKeywordConstructor,
        eKeywordInt,
        eKeywordBoolean,
        eKeywordChar,
        eKeywordVoid,
        eKeywordVar,
        eKeywordStatic,
        eKeywordField,
        eKeywordLet,
        eKeywordDo,
        eKeywordIf,
        eKeywordElse,
        eKeywordWhile,
        eKeywordReturn,
        eKeywordTrue,
        eKeywordFalse,
        eKeywordNull,
        eKeywordThis,
        eKeywordCount
    };

    class JackTokenizer
    {
    public:
        JackTokenizer(const std::string& filename)
            : inputStream(filename),
              tokenType(eTokenTypeNone),
              keywordType(eKeywordNone),
              keyword(std::string()),
              symbol(0),
              identifier(std::string()),
              intVal(0),
              stringVal(std::string()),
              lineNumber(1),
              isTokenPreserved(false) {}

        bool HasMoreTokens();
        void Advance();
        eTokenType GetTokenType() const;
        eKeyword GetKeywordType() const;
        std::string GetKeyword() const;
        char GetSymbol() const;
        std::string GetIdentifier() const;
        int GetIntVal() const;
        std::string GetStringVal() const;
        void ThrowException(const std::string& exMsg);
        void PutbackToken();
    private:
        std::ifstream inputStream;
        eTokenType tokenType;
        eKeyword keywordType;
        std::string keyword;
        std::string keywordStr;
        char symbol;
        std::string identifier;
        int intVal;
        std::string stringVal;
        size_t lineNumber;
        bool isTokenPreserved;

        void SetTokenType(eTokenType tokenType);
        void SetKeyword(const std::string& strVal);
        void SetSymbol(char symbol);
        void SetIdentifier(std::string identifier);
        void SetIntVal(int intVal);
        void setStringVal(std::string stringVal);
        void SetDefaultOutputs();

        void GetNextSymbol(char& symbol);
        bool SkipComment();
        void PutbackSymbol(char symbol);
        void ExtractSymbol(char currentSymbol);
        void ExtractStringConst(char currentSymbol);
        void ExtractIntValue(char currentSymbol);
        void ExtractIdentifier(char currentSymbol);
    };
}

#endif // JACK_TOKENIZER_H
