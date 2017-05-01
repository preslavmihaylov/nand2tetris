#include <cassert>

#include "VMWriter.h"

using namespace std;
using namespace JackCompiler;

string GetOperationString(eVMOperation operation)
{
    string result;
    switch (operation)
    {
        case eVMOperationAdd:
            result = "add";
            break;
        case eVMOperationSub:
            result = "sub";
            break;
        case eVMOperationNeg:
            result = "neg";
            break;
        case eVMOperationEq:
            result = "eq";
            break;
        case eVMOperationGt:
            result = "gt";
            break;
        case eVMOperationLt:
            result = "lt";
            break;
        case eVMOperationAnd:
            result = "and";
            break;
        case eVMOperationOr:
            result = "or";
            break;
        case eVMOperationNot:
            result = "not";
            break;
        default:
            // shouldn't invoke this with a wrong operation
            assert(false);
    }

    return result;
}

string GetSegmentString(eVMSegment segmentType)
{
    string result;
    switch (segmentType)
    {
        case eVMSegmentArg:
            result = "argument";
            break;
        case eVMSegmentConst:
            result = "constant";
            break;
        case eVMSegmentLocal:
            result = "local";
            break;
        case eVMSegmentStatic:
            result = "static";
            break;
        case eVMSegmentThis:
            result = "this";
            break;
        case eVMSegmentThat:
            result = "that";
            break;
        case eVMSegmentPointer:
            result = "pointer";
            break;
        case eVMSegmentTemp:
            result = "temp";
            break;
        default:
            assert(false);
    }

    return result;
}

void VMWriter::WritePush(eVMSegment segmentType, size_t index)
{
    outputStream << "push " << GetSegmentString(segmentType) << " " << index << endl;
}

void VMWriter::WritePop(eVMSegment segmentType, size_t index)
{
    outputStream << "pop " << GetSegmentString(segmentType) << " " << index << endl;
}

void VMWriter::WriteArithmetic(eVMOperation operation)
{

    outputStream << GetOperationString(operation) << endl;
}

void VMWriter::WriteLabel(const std::string& label)
{
    outputStream << "label " << label << endl;
}

void VMWriter::WriteGoto(const std::string& label)
{
    outputStream << "goto " << label << endl;
}

void VMWriter::WriteIfGoto(const std::string& label)
{
    outputStream << "if-goto " << label << endl;
}

void VMWriter::WriteCall(const std::string& subName, size_t argsCnt)
{
    outputStream << "call " << subName << " " << argsCnt << endl;
}

void VMWriter::WriteFunction(const std::string& subName, size_t localsCnt)
{
    outputStream << "function " << subName << " " << localsCnt << endl;
}

void VMWriter::WriteReturn()
{
    outputStream << "return" << endl;
}
