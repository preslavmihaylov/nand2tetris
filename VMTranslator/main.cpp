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

    string filename(argv[1]);

    int dotIndex = filename.find_last_of(".");
    if (dotIndex != -1)
    {
        vmt.TranslateFile(filename);
    }
    else
    {
        vmt.TranslateDirectory(filename);
    }

    return 0;
}
