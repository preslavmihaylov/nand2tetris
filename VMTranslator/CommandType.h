#ifndef COMMANDTYPE_H
#define COMMANDTYPE_H

namespace HackVMTranslator
{
    enum eCommandType
    {
        eCommandTypeEmpty,
        eCommandTypeArithmetic,
        eCommandTypePush,
        eCommandTypePop,
        eCommandTypeLabel,
        eCommandTypeGoto,
        eCommandTypeIf,
        eCommandTypeFunction,
        eCommandTypeReturn,
        eCommandTypeCall
    };
}

#endif // COMMANDTYPE_H
