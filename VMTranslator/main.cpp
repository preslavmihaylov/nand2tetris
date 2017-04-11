#include <iostream>

#include "VMTranslator.h"

using namespace std;
using namespace HackVMTranslator;

int main(int argc, char* argv[])
{
    VMTranslator vmt;

    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <input filename>.vm" << endl;
        return 1;
    }

    vmt.Translate(argv[1]);
    return 0;
}
