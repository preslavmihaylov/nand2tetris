#include "symbol-table.h"

SymbolTable::SymbolTable()
{
    // Predefined symbols
    this->symbolTable =
    {
        {"R0",      0},
        {"R1",      1},
        {"R2",      2},
        {"R3",      3},
        {"R4",      4},
        {"R5",      5},
        {"R6",      6},
        {"R7",      7},
        {"R8",      8},
        {"R9",      9},
        {"R10",     10},
        {"R11",     11},
        {"R12",     12},
        {"R13",     13},
        {"R14",     14},
        {"R15",     15},
        {"SCREEN",  16384},
        {"KBD",     24576},
        {"SP",      0},
        {"LCL",     1},
        {"ARG",     2},
        {"THIS",    3},
        {"THAT",    4},
    };
}

bool SymbolTable::SymbolExists(string symbol)
{
    return this->symbolTable.find(symbol) != this->symbolTable.end();
}

void SymbolTable::AddSymbol(string symbol)
{
    int index = this->GetNextIndex();
    this->AddSymbol(symbol, index);
}

void SymbolTable::AddSymbol(string symbol, int index)
{
    if (!this->SymbolExists(symbol))
    {
        this->symbolTable[symbol] = index;
    }
}

int SymbolTable::GetSymbol(string symbol)
{
    if (this->SymbolExists(symbol))
    {
        return this->symbolTable[symbol];
    }

    return -1;
}

int SymbolTable::GetNextIndex()
{
    return this->currentIndex++;
}
