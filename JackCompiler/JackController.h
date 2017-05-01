#ifndef JACK_CONTROLLER_H
#define JACK_CONTROLLER_H

#include <string>

namespace JackCompiler
{
    class JackController
    {
    public:
        void CompileDirectory(const std::string& directoryName);
        void CompileFile(const std::string& inputFilename);
    private:
        void CompileFileInternal(const std::string& inputFilename);
    };
}

#endif // JACK_CONTROLLER_H
