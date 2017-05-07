#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>

//#define DEBUG_BUILD

namespace JackCompiler
{
    #define INVALID_NUMBER (-1)

    enum eVariableKind
    {
        eVariableKindStatic,
        eVariableKindField,
        eVariableKindArgument,
        eVariableKindLocal,
        eVariableKindCount,
        eVariableKindNone
    };

    struct VariableInfo
    {
        std::string type;
        eVariableKind kind;
        int index;

        VariableInfo(const std::string& type, eVariableKind kind, int index)
            : type(type), kind(kind), index(index) {};
    };

    typedef std::unordered_map<std::string, VariableInfo> SymbolHashTable;
    class SymbolTable
    {
    public:
        SymbolTable() {};
        void StartSubroutine();
        void Define(const std::string& identifier, const std::string& type, eVariableKind kind);
        int VarCount(eVariableKind kind);
        eVariableKind GetKindOf(const std::string& identifier);
        std::string GetTypeOf(const std::string& identifier);
        int GetIndexOf(const std::string& identifier);
        void Clear();
    private:
        SymbolHashTable classTable;
        SymbolHashTable subroutineTable;
        int indicesTracker[eVariableKindCount] = { 0 }; // all entries are zero

        void StoreVariable(SymbolHashTable& table, const std::string& identifier, const VariableInfo& info);
        SymbolHashTable* GetIdentifierScope(const std::string& identifier);
    };
}

#endif // SYMBOL_TABLE_H
