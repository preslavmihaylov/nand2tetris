#include <cassert>

#include "SymbolTable.h"

using namespace std;
using namespace JackCompiler;

/* PRIVATE METHODS */
void SymbolTable::StoreVariable(SymbolHashTable& table, const std::string& identifier, const VariableInfo& info)
{
    table.insert({identifier, info});
}

SymbolHashTable* SymbolTable::GetIdentifierScope(const std::string& identifier)
{
    if (this->subroutineTable.find(identifier) != this->subroutineTable.end())
    {
        return &this->subroutineTable;
    }
    else if (this->classTable.find(identifier) != this->classTable.end())
    {
        return &this->classTable;
    }
    else
    {
        return nullptr;
    }
}

/* PUBLIC METHODS */
void SymbolTable::StartSubroutine()
{
    this->subroutineTable.clear();
    this->indicesTracker[eVariableKindArgument] = 0;
    this->indicesTracker[eVariableKindLocal] = 0;
}

void SymbolTable::Define(const std::string& identifier, const std::string& type, eVariableKind kind)
{
    if (kind >= eVariableKindCount)
    {
        assert(false);
        return;
    }

    VariableInfo varInfo(type, kind, this->indicesTracker[kind]);
    ++this->indicesTracker[kind]; // increment next index

    switch (kind)
    {
    case eVariableKindArgument:
    case eVariableKindLocal:
        this->StoreVariable(this->subroutineTable, identifier, varInfo);
        break;
    case eVariableKindField:
    case eVariableKindStatic:
        this->StoreVariable(this->classTable, identifier, varInfo);
        break;
    default:
        assert(false);
    }
}

int SymbolTable::VarCount(eVariableKind kind)
{
    if (kind >= eVariableKindCount)
    {
        // no such kind exists
        assert(false);
        return INVALID_NUMBER;
    }

    return this->indicesTracker[kind];
}

eVariableKind SymbolTable::GetKindOf(const std::string& identifier)
{
    SymbolHashTable* table = this->GetIdentifierScope(identifier);
    if (table != nullptr)
    {
        return table->at(identifier).kind;
    }
    else
    {
        return eVariableKindNone;
    }
}

std::string SymbolTable::GetTypeOf(const std::string& identifier)
{
    SymbolHashTable* table = this->GetIdentifierScope(identifier);
    if (table != nullptr)
    {
        return table->at(identifier).type;
    }
    else
    {
        // empty string
        return string();
    }
}

int SymbolTable::GetIndexOf(const std::string& identifier)
{
    SymbolHashTable* table = this->GetIdentifierScope(identifier);
    if (table != nullptr)
    {
        return table->at(identifier).index;
    }
    else
    {
        return INVALID_NUMBER;
    }
}

void SymbolTable::Clear()
{
    this->classTable.clear();
    this->subroutineTable.clear();
    for (int i = 0; i < eVariableKindCount; ++i)
    {
        this->indicesTracker[i] = 0;
    }
}
