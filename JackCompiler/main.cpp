#include <iostream>

#include "JackController.h"

using namespace std;
using namespace JackCompiler;

int main(int argc, char * argv[])
{
    //return 0;

    if (argc != 2)
    {
        cerr << "usage: " << argv[0] << " <input file>.jack" << endl;
        return 1;
    }

    JackController jackController;
    string filename(argv[1]);
    size_t dotIndex = filename.find_last_of(".");

    if (dotIndex != string::npos)
    {
        jackController.CompileFile(filename);
    }
    else
    {
        jackController.CompileDirectory(filename);
    }

    return 0;
}
