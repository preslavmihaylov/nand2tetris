#include <iostream>
#include <sstream>
#include <cassert>

#include "CodeWriter.h"

using namespace std;
using namespace HackVMTranslator;

/* PRIVATE METHODS */
void CodeWriter::CompileAddCommand()
{
    this->outputDest << "// add" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M-1 // SP--" << endl;
    this->outputDest << "A=M" << endl;
    this->outputDest << "D=M   // D = (*SP)" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=D+M // *(SP-1) = D + *(SP-1)" << endl;
    this->outputDest << endl;
}

void CodeWriter::CompileSubtractCommand()
{
    this->outputDest << "// sub" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M-1 // SP--" << endl;
    this->outputDest << "A=M" << endl;
    this->outputDest << "D=M   // D = *SP" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=M-D // *(SP-1) = *(SP-1) - D" << endl;
    this->outputDest << endl;
}

void CodeWriter::CompileNegateCommand()
{
    this->outputDest << "// neg" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=-M  // (*(SP-1)) = -(*(SP-1))" << endl;
    this->outputDest << endl;
}

void CodeWriter::CompileEqualsCommand()
{
    ostringstream oss1;
    ostringstream oss2;

    oss1 << this->outputFilename << ".TRUE." << this->labelsCounter;
    string trueLabel = oss1.str();

    oss2 << this->outputFilename << ".END." << this->labelsCounter;
    string endLabel = oss2.str();

    this->labelsCounter += 1;

    this->outputDest << "// eq" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M-1 // SP--" << endl;
    this->outputDest << "A=M" << endl;
    this->outputDest << "D=M   // D = *SP" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "D=M-D // D = *(SP-1) - D" << endl;
    this->outputDest << "@" << trueLabel << endl;
    this->outputDest << "D; JEQ // if (D == 0) goto TRUE" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=0   // *(SP-1) = false" << endl;
    this->outputDest << "@" << endLabel << endl;
    this->outputDest << "0; JMP // goto END" << endl;
    this->outputDest << "(" << trueLabel << ")" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=-1   // *(SP-1) = true" << endl;
    this->outputDest << "(" + endLabel << ")" << endl;
    this->outputDest << endl;
}

void CodeWriter::CompileGreaterThanCommand()
{
    ostringstream oss1;
    ostringstream oss2;

    oss1 << this->outputFilename << ".TRUE." << this->labelsCounter;
    string trueLabel = oss1.str();

    oss2 << this->outputFilename << ".END." << this->labelsCounter;
    string endLabel = oss2.str();

    this->labelsCounter += 1;

    this->outputDest << "// gt" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M-1 // SP--" << endl;
    this->outputDest << "A=M" << endl;
    this->outputDest << "D=M   // D = *SP" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "D=M-D // D = *(SP-1) - D" << endl;
    this->outputDest << "@" << trueLabel << endl;
    this->outputDest << "D; JGT // if (D > 0) goto TRUE" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=0   // *(SP-1) = false" << endl;
    this->outputDest << "@" << endLabel << endl;
    this->outputDest << "0; JMP // goto END" << endl;
    this->outputDest << "(" << trueLabel << ")" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=-1   // *(SP-1) = true" << endl;
    this->outputDest << "(" + endLabel << ")" << endl;
    this->outputDest << endl;
}

void CodeWriter::CompileLessThanCommand()
{
    ostringstream oss1;
    ostringstream oss2;

    oss1 << this->outputFilename << ".TRUE." << this->labelsCounter;
    string trueLabel = oss1.str();

    oss2 << this->outputFilename << ".END." << this->labelsCounter;
    string endLabel = oss2.str();

    this->labelsCounter += 1;

    this->outputDest << "// lt" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M-1 // SP--" << endl;
    this->outputDest << "A=M" << endl;
    this->outputDest << "D=M   // D = *SP" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "D=M-D // D = *(SP-1) - D" << endl;
    this->outputDest << "@" << trueLabel << endl;
    this->outputDest << "D; JLT // if (D < 0) goto TRUE" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=0   // *(SP-1) = false" << endl;
    this->outputDest << "@" << endLabel << endl;
    this->outputDest << "0; JMP // goto END" << endl;
    this->outputDest << "(" << trueLabel << ")" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=-1   // *(SP-1) = true" << endl;
    this->outputDest << "(" + endLabel << ")" << endl;
    this->outputDest << endl;
}

void CodeWriter::CompileAndCommand()
{
    this->outputDest << "// and" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M-1 // SP--" << endl;
    this->outputDest << "A=M" << endl;
    this->outputDest << "D=M   // D = *SP" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=D&M // *(SP-1) = D&(*(SP-1))" << endl;
    this->outputDest << endl;
}

void CodeWriter::CompileOrCommand()
{
    this->outputDest << "// or" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M-1 // SP--" << endl;
    this->outputDest << "A=M" << endl;
    this->outputDest << "D=M   // D = *SP" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=D|M // *(SP-1) = D|(*(SP-1))" << endl;
    this->outputDest << endl;
}

void CodeWriter::CompileNotCommand()
{
    this->outputDest << "// not" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=!M  // *(SP-1)=!(*(SP-1))" << endl;
    this->outputDest << endl;
}

void CodeWriter::CompileSimplePushCommand(const string& segmentLabel, int index)
{
    if (segmentLabel == "R5")
    {
        this->outputDest << "// push temp " << index << endl;
    }
    else
    {
        this->outputDest << "// push " << segmentLabel << " " << index << endl;
    }

    this->outputDest << "@" << index << endl;
    this->outputDest << "D=A   // D=i" << endl;
    this->outputDest << "@" << segmentLabel << endl;

    if (segmentLabel == "R5")
    {
        this->outputDest << "A=A+D" << endl;
    }
    else
    {
        this->outputDest << "A=M+D" << endl;
    }

    this->outputDest << "D=M   // D=*(segment+i)" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M+1 // SP++" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=D   // *(SP-1)=D" << endl;
    this->outputDest << endl;
}

void CodeWriter::CompileConstantPushCommand(int index)
{
    this->outputDest << "// push constant " << index << endl;
    this->outputDest << "@" << index << endl;
    this->outputDest << "D=A   // D=i" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M+1 // SP++" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=D   // *(SP-1)=D" << endl;
    this->outputDest << endl;
}

void CodeWriter::CompileStaticPushCommand(int index)
{
    ostringstream oss;

    oss << this->outputFilename << "." << index;
    string staticLabel = oss.str();

    this->outputDest << "// push static " << index << endl;
    this->outputDest << "@" << staticLabel << endl;
    this->outputDest << "D=M   // D = *(static+i)" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M+1 // SP++" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=D   // *(SP-1)=D" << endl;
    this->outputDest << endl;
}

void CodeWriter::CompilePointerPushCommand(int index)
{
    string label;
    if (index == 0)
    {
        label = "THIS";
    }
    else if (index == 1)
    {
        label = "THAT";
    }
    else
    {
        // At this point, code should be valid
        assert(false);
        return;
    }

    this->outputDest << "// push pointer " << index << endl;
    this->outputDest << "@" << label << endl;
    this->outputDest << "D=M   // D=" << label << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M+1 // SP++" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=D   // *(SP-1)=D" << endl;
    this->outputDest << endl;
}

void CodeWriter::CompileSimplePopCommand(const string& segment, int index)
{
    // temp segment
    if (segment == "R5")
    {
        this->outputDest << "// pop temp " << index << endl;
    }
    else
    {
        this->outputDest << "// pop " << segment << " " << index << endl;
    }
    this->outputDest << "@" << segment << endl;

    if (segment == "R5")
    {
        this->outputDest << "D=A   // D=" << segment << endl;
    }
    else
    {
        this->outputDest << "D=M   // D=" << segment << endl;
    }

    this->outputDest << "@" << index << endl;
    this->outputDest << "D=D+A // D=D+i" << endl;
    this->outputDest << "@R15" << endl;
    this->outputDest << "M=D   // R15=D" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M-1 // SP--" << endl;
    this->outputDest << "A=M" << endl;
    this->outputDest << "D=M   // D=(*SP)" << endl;
    this->outputDest << "@R15" << endl;
    this->outputDest << "A=M" << endl;
    this->outputDest << "M=D   // *(" << segment << "+i)=D" << endl;
    this->outputDest << endl;
}

void CodeWriter::CompileStaticPopCommand(int index)
{
    ostringstream oss;

    oss << this->outputFilename << "." << index;
    string staticLabel = oss.str();

    this->outputDest << "// pop static " << index << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M-1 // SP--" << endl;
    this->outputDest << "A=M" << endl;
    this->outputDest << "D=M   // D=(*SP)" << endl;
    this->outputDest << "@" << staticLabel << endl;
    this->outputDest << "M=D   // *(static+i)=D" << endl;
    this->outputDest << endl;
}

void CodeWriter::CompilePointerPopCommand(int index)
{
    string label;
    if (index == 0)
    {
        label = "THIS";
    }
    else if (index == 1)
    {
        label = "THAT";
    }
    else
    {
        // At this point, code should be valid
        assert(false);
        return;
    }

    this->outputDest << "// pop pointer " << index << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M-1 // SP--" << endl;
    this->outputDest << "A=M" << endl;
    this->outputDest << "D=M  // D=(*SP)" << endl;
    this->outputDest << "@" << label << endl;
    this->outputDest << "M=D  // " << label << "=D" << endl;
    this->outputDest << endl;
}

/* PUBLIC METHODS */
void CodeWriter::WriteArithmetic(const std::string& command)
{
    if (!this->outputDest.is_open())
    {
        return;
    }

    if (command == "add")
    {
        this->CompileAddCommand();
    }
    else if (command == "sub")
    {
        this->CompileSubtractCommand();
    }
    else if (command == "neg")
    {
        this->CompileNegateCommand();
    }
    else if (command == "eq")
    {
        this->CompileEqualsCommand();
    }
    else if (command == "gt")
    {
        this->CompileGreaterThanCommand();
    }
    else if (command == "lt")
    {
        this->CompileLessThanCommand();
    }
    else if (command == "and")
    {
        this->CompileAndCommand();
    }
    else if (command == "or")
    {
        this->CompileOrCommand();
    }
    else if (command == "not")
    {
        this->CompileNotCommand();
    }
    else
    {
        // At this point, command should be valid
        assert(false);
    }
}

void CodeWriter::WritePushPop(eCommandType commandType, const std::string& segment, int index)
{
    if (!this->outputDest.is_open())
    {
        return;
    }

    if (commandType == eCommandTypePush)
    {
        if (segment == "local")
        {
            this->CompileSimplePushCommand("LCL", index);
        }
        else if (segment == "argument")
        {
            this->CompileSimplePushCommand("ARG", index);
        }
        else if (segment == "this")
        {
            this->CompileSimplePushCommand("THIS", index);
        }
        else if (segment == "that")
        {
            this->CompileSimplePushCommand("THAT", index);
        }
        else if (segment == "constant")
        {
            this->CompileConstantPushCommand(index);
        }
        else if (segment == "static")
        {
            this->CompileStaticPushCommand(index);
        }
        else if (segment == "temp")
        {
            // temp starts from R5
            this->CompileSimplePushCommand("R5", index);
        }
        else if (segment == "pointer")
        {
            if (index == 0 || index == 1)
            {
                this->CompilePointerPushCommand(index);
            }
            else
            {
                // At this point, command should be valid
                assert(false);
            }
        }
        else
        {
            // At this point, command should be valid
            assert(false);
        }
    }
    else // if (commandType == eCommandTypePop)
    {
        if (segment == "local")
        {
            this->CompileSimplePopCommand("LCL", index);
        }
        else if (segment == "argument")
        {
            this->CompileSimplePopCommand("ARG", index);
        }
        else if (segment == "this")
        {
            this->CompileSimplePopCommand("THIS", index);
        }
        else if (segment == "that")
        {
            this->CompileSimplePopCommand("THAT", index);
        }
        else if (segment == "static")
        {
            this->CompileStaticPopCommand(index);
        }
        else if (segment == "temp")
        {
            // temp begins from R5
            this->CompileSimplePopCommand("R5", index);
        }
        else if (segment == "pointer")
        {
            if (index == 0 || index == 1)
            {
                this->CompilePointerPopCommand(index);
            }
            else
            {
                // At this point, command should be valid
                assert(false);
            }
        }
        else
        {
            // At this point, command should be valid
            assert(false);
        }
    }
}
