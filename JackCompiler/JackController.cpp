#include <dirent.h>
#include <iostream>

#include "JackController.h"
#include "CompilationEngine.h"

using namespace std;
using namespace JackCompiler;

bool DoesFileExist(const string& filename)
{
    ifstream infile(filename);
    return infile.good();
}

bool IsValidJackFile(const string& filename)
{
    int dotIndex = filename.find_last_of(".");
    return (filename.substr(dotIndex + 1) == "jack");
}

/* PRIVATE METHODS */
void JackController::CompileFileInternal(const string& inputFilename)
{
    int dotIndex = inputFilename.find_last_of(".");
    string outputFilename = inputFilename.substr(0, dotIndex);
    string outputFilenameWithExtension = outputFilename + ".vm";

    CompilationEngine compiler(inputFilename, outputFilenameWithExtension);
    compiler.CompileFile();
}

/* PUBLIC METHODS */
void JackController::CompileDirectory(const string& directoryName)
{
    struct dirent *entry;
    DIR *dp;
    string currentFilename;

    dp = opendir(directoryName.c_str());
    if (dp == NULL) {
        cerr << ("Specified Path does not exist or could not be read.");
        return;
    }

    while ((entry = readdir(dp)))
    {
        currentFilename = entry->d_name;
        if (IsValidJackFile(currentFilename))
        {
            this->CompileFileInternal(directoryName + "/" + currentFilename);
        }
    }

    closedir(dp);
}

void JackController::CompileFile(const string& inputFilename)
{
    if (!IsValidJackFile(inputFilename))
    {
        cerr << "Invalid file extension: " << inputFilename << endl;
        return;
    }
    else if (!DoesFileExist(inputFilename))
    {
        cerr << "The specified file does not exist: " << inputFilename << endl;
        return;
    }

    this->CompileFileInternal(inputFilename);
}
