#ifndef JACK_COMPILER_H
#define JACK_COMPILER_H

#include <string>

namespace JackCompiler
{
    class JackAnalyzer
    {
    public:
        void CompileDirectory(const std::string& directoryName);
        void CompileFile(const std::string& inputFilename);
    private:
        void CompileFileInternal(const std::string& inputFilename);
    };
}

#endif // JACK_COMPILER_H
