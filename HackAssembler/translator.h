#ifndef TRANSLATOR_H

#include <string>

using namespace std;

class Translator
{
    public:
        string TranslateAInstruction(string value);
        string TranslateCInstruction(string dest, string comp, string jump);
};

#endif // TRANSLATOR_H
