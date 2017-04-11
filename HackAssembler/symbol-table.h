#ifndef SYMBOL_TABLE_H

#include <map>

using namespace std;

// For easier use of Singleton class
#define SYMBOL_TABLE (SymbolTable::GetInstance())

class SymbolTable
{
    public:
        static SymbolTable& GetInstance()
        {
            static SymbolTable instance;
            return instance;
        };
        bool SymbolExists(string symbol);
        void AddSymbol(string symbol);
        void AddSymbol(string symbol, int value);
        int GetSymbol(string symbol);
        void Print();

        // Deleted methods due to Singleton
        SymbolTable(SymbolTable const&)    = delete;
        void operator=(SymbolTable const&) = delete;

    private:
        SymbolTable();
        map<string, int> symbolTable;
        int currentIndex = 16;
        int GetNextIndex();
};

#endif // SYMBOL_TABLE_H
