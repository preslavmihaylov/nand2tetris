#include <iostream>
#include <stdexcept>

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

    try
    {
        if (dotIndex != string::npos)
        {
            jackController.CompileFile(filename);
        }
        else
        {
            jackController.CompileDirectory(filename);
        }
    }
    catch (const runtime_error& ex)
    {
        cerr << "Error: " << ex.what();
    }

    return 0;
}
