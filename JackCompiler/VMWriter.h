#ifndef VMWRITER_H
#define VMWRITER_H

#include <fstream>
#include <string>

namespace JackCompiler
{
    enum eVMSegment
    {
        eVMSegmentConst,
        eVMSegmentArg,
        eVMSegmentLocal,
        eVMSegmentStatic,
        eVMSegmentThis,
        eVMSegmentThat,
        eVMSegmentPointer,
        eVMSegmentTemp,
        eVMSegmentCount,
        eVMSegmentNone
    };

    enum eVMOperation
    {
        eVMOperationAdd,
        eVMOperationSub,
        eVMOperationNeg,
        eVMOperationEq,
        eVMOperationGt,
        eVMOperationLt,
        eVMOperationAnd,
        eVMOperationOr,
        eVMOperationNot,
        eVMOperationCount,
    };

    class VMWriter
    {
    public:
        VMWriter(const std::string& outputFilename)
            : outputStream(outputFilename) {};

        void WritePush(eVMSegment segmentType, size_t index);
        void WritePop(eVMSegment segmentType, size_t index);
        void WriteArithmetic(eVMOperation operation);
        void WriteLabel(const std::string& label);
        void WriteGoto(const std::string& label);
        void WriteIfGoto(const std::string& label);
        void WriteCall(const std::string& subName, size_t argsCnt);
        void WriteFunction(const std::string& subName, size_t localsCnt);
        void WriteReturn();
    private:
        std::ofstream outputStream;
    };
}

#endif // VMWRITER_H
