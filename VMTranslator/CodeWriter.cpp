#include <iostream>
#include <sstream>
#include <cassert>

#include "CodeWriter.h"

using namespace std;
using namespace HackVMTranslator;

/* PRIVATE METHODS */
void CodeWriter::CompileSumationCommand(const string& command)
{
    string sumOp;
    if (command == "add")
    {
        sumOp = "M+D";
    }
    else if (command == "sub")
    {
        sumOp = "M-D";
    }
    else
    {
        // command should be valid by this point
        assert(false);
        return;
    }

    this->outputDest << "// " << command << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M-1 // SP--" << endl;
    this->outputDest << "A=M" << endl;
    this->outputDest << "D=M   // D = (*SP)" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=" << sumOp << " // *(SP-1) = D " << command << " *(SP-1)" << endl;
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

void CodeWriter::CompileComparisonCommand(const string& command)
{
    ostringstream oss1;
    ostringstream oss2;

    oss1 << this->processedFilename << ".TRUE." << this->labelsCounter;
    string trueLabel = oss1.str();

    oss2 << this->processedFilename << ".END." << this->labelsCounter;
    string endLabel = oss2.str();

    this->labelsCounter += 1;

    string jumpInstruction;
    if (command == "eq")
    {
        jumpInstruction = "JEQ";
    }
    else if (command == "gt")
    {
        jumpInstruction = "JGT";
    }
    else if (command == "lt")
    {
        jumpInstruction = "JLT";
    }
    else
    {
        // command should be valid by this point
        assert(false);
        return;
    }

    this->outputDest << "// " << command << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M-1 // SP--" << endl;
    this->outputDest << "A=M" << endl;
    this->outputDest << "D=M   // D = *SP" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "D=M-D // D = *(SP-1) - D" << endl;
    this->outputDest << "@" << trueLabel << endl;
    this->outputDest << "D; " << jumpInstruction << " // if (D " << command << " 0) goto TRUE" << endl;
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

void CodeWriter::CompileLogicalCommand(const string& command)
{
    string op;
    if (command == "and")
    {
        op = "&";
    }
    else if (command == "or")
    {
        op = "|";
    }
    else
    {
        // code should be valid by this point
        assert(false);
        return;
    }

    this->outputDest << "// " << command << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M-1 // SP--" << endl;
    this->outputDest << "A=M" << endl;
    this->outputDest << "D=M   // D = *SP" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=D" << op << "M // *(SP-1) = D" << op << "(*(SP-1))" << endl;
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

    oss << this->processedFilename << "." << index;
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

    oss << this->processedFilename << "." << index;
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

void CodeWriter::CompileLabelCommand(const std::string& label)
{
    string updatedLabel = this->processedFilename + "." + this->processedFunction + "$" + label;
    this->outputDest << "// label " << label << endl;
    this->outputDest << "(" << updatedLabel << ")" << endl;
    this->outputDest << endl;
}

void CodeWriter::CompileGotoCommand(const std::string& label)
{
    string updatedLabel = this->processedFilename + "." + this->processedFunction + "$" + label;
    this->outputDest << "// goto " << label << endl;
    this->outputDest << "@" << updatedLabel << endl;
    this->outputDest << "0; JMP" << endl;
    this->outputDest << endl;
}

void CodeWriter::CompileIfCommand(const std::string& label)
{
    string updatedLabel = this->processedFilename + "." + this->processedFunction + "$" + label;
    this->outputDest << "// if-goto " << label << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M-1 // SP--" << endl;
    this->outputDest << "A=M" << endl;
    this->outputDest << "D=M   // D=(*SP)" << endl;
    this->outputDest << "@" << updatedLabel << endl;
    this->outputDest << "D; JNE // if (D != 0) goto " << label << endl;
    this->outputDest << endl;
}

void CodeWriter::CompileCallCommand(const std::string& functionName, int argsCnt)
{
    ostringstream oss;
    oss << this->processedFilename << "." << functionName << "$return" << this->labelsCounter;
    string returnLabel = oss.str();
    ++this->labelsCounter;

    // store return address to stack frame
    this->outputDest << "// call " << functionName << " " << argsCnt << endl;
    this->outputDest << "@" << returnLabel << endl;
    this->outputDest << "D=A" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M+1 // SP++" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=D   // M=(address for return)" << endl;

    // store LCL to stack frame
    this->outputDest << "@LCL  // store LCL to stack frame" << endl;
    this->outputDest << "D=M   // D=LCL" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M+1 // SP++" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=D   // *(SP-1)=LCL" << endl;

    // store ARG to stack frame
    this->outputDest << "@ARG  // store ARG to stack frame" << endl;
    this->outputDest << "D=M   // D=ARG" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M+1 // SP++" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=D   // *(SP-1)=ARG" << endl;

    // store THIS to stack frame
    this->outputDest << "@THIS // store THIS to stack frame" << endl;
    this->outputDest << "D=M   // D=THIS" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M+1 // SP++" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=D   // *(SP-1)=THIS" << endl;

    // store THAT to stack frame
    this->outputDest << "@THAT // store THAT to stack frame" << endl;
    this->outputDest << "D=M   // D=THAT" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M+1 // SP++" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=D   // *(SP-1)=THAT" << endl;

    // set ARG to (SP-5-argsCnt)
    this->outputDest << "@SP   // set ARG=(SP-5-argsCnt)" << endl;
    this->outputDest << "D=M   // D=SP" << endl;
    this->outputDest << "@5" << endl;
    this->outputDest << "D=D-A // D=D-5" << endl;
    this->outputDest << "@" << argsCnt << endl;
    this->outputDest << "D=D-A // D=D-argsCnt" << endl;
    this->outputDest << "@ARG" << endl;
    this->outputDest << "M=D   // ARG=(SP-5-argsCnt)" << endl;

    // set LCL=SP
    this->outputDest << "@SP   // set LCL=SP" << endl;
    this->outputDest << "D=M" << endl;
    this->outputDest << "@LCL" << endl;
    this->outputDest << "M=D   // LCL=SP" << endl;
    // call function
    this->outputDest << "@" << functionName  << " // jump to function" << endl;
    this->outputDest << "0; JMP" << endl;
    this->outputDest << "(" << returnLabel << ")" << endl;

    this->outputDest << endl;
}

void CodeWriter::CompileFunctionCommand(const std::string& functionName, int localsCnt)
{
    ostringstream oss;
    oss << this->processedFilename << "." << functionName << ".LOOP." << this->labelsCounter;
    string funcLoopLabel = oss.str();
    oss << ".END";
    string funcLoopEndLabel = oss.str();

    this->labelsCounter += 1;

    // update processed function
    this->processedFunction = functionName;

    this->outputDest << "(" << functionName << ")" << endl;
    this->outputDest << "@" << localsCnt << " // arguments count" << endl;
    this->outputDest << "D=A" << endl;

    // Loop for initializing the local segment
    this->outputDest << "(" << funcLoopLabel << ")" << endl;
    this->outputDest << "@" << funcLoopEndLabel << endl;
    this->outputDest << "D; JLE // if (D <= 0) goto " << funcLoopEndLabel << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=M+1  // SP++" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "M=0    // *(SP-1)=0" << endl;
    this->outputDest << "D=D-1  // D--" << endl;
    this->outputDest << "@" << funcLoopLabel << endl;
    this->outputDest << "0; JMP" << endl;
    this->outputDest << "(" << funcLoopEndLabel << ")" << endl;
    this->outputDest << endl;
}

void CodeWriter::CompileReturnCommand()
{
    // store end of stack frame (LCL) to R6
    this->outputDest << "@LCL" << endl;
    this->outputDest << "D=M" << endl;
    this->outputDest << "@R6" << endl;
    this->outputDest << "M=D    // *R6 = LCL, store end of stack frame" << endl;

    // store return address (LCL-5) to R7
    this->outputDest << "@5" << endl;
    this->outputDest << "D=D-A" << endl;
    this->outputDest << "A=D" << endl;
    this->outputDest << "D=M    // D=*(LCL-5)" << endl;
    this->outputDest << "@R7" << endl;
    this->outputDest << "M=D    // store return address to R7" << endl;

    // store returned value
    this->outputDest << "@SP" << endl;
    this->outputDest << "A=M-1" << endl;
    this->outputDest << "D=M    // D=*(SP-1)" << endl;
    this->outputDest << "@ARG" << endl;
    this->outputDest << "A=M" << endl;
    this->outputDest << "M=D    // *ARG=D, store returned value" << endl;

    // adjust caller stack pointer
    this->outputDest << "D=A" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=D+1  // SP=(ARG+1)" << endl;

    // restore caller THAT address
    this->outputDest << "@R6" << endl;
    this->outputDest << "D=M" << endl;
    this->outputDest << "@1" << endl;
    this->outputDest << "D=D-A" << endl;
    this->outputDest << "A=D" << endl;
    this->outputDest << "D=M    // D=*(R6-1), caller THAT addr" << endl;
    this->outputDest << "@THAT" << endl;
    this->outputDest << "M=D" << endl;

    // restore caller THIS address
    this->outputDest << "@R6" << endl;
    this->outputDest << "D=M" << endl;
    this->outputDest << "@2" << endl;
    this->outputDest << "D=D-A" << endl;
    this->outputDest << "A=D" << endl;
    this->outputDest << "D=M    // D=*(R6-2), caller THIS addr" << endl;
    this->outputDest << "@THIS" << endl;
    this->outputDest << "M=D" << endl;

    // restore caller ARG address
    this->outputDest << "@R6" << endl;
    this->outputDest << "D=M" << endl;
    this->outputDest << "@3" << endl;
    this->outputDest << "D=D-A" << endl;
    this->outputDest << "A=D" << endl;
    this->outputDest << "D=M    // D=*(R6-3), caller ARG addr" << endl;
    this->outputDest << "@ARG" << endl;
    this->outputDest << "M=D" << endl;

    // restore caller THAT address
    this->outputDest << "@R6" << endl;
    this->outputDest << "D=M" << endl;
    this->outputDest << "@4" << endl;
    this->outputDest << "D=D-A" << endl;
    this->outputDest << "A=D" << endl;
    this->outputDest << "D=M    // D=*(R6-4), caller LCL addr" << endl;
    this->outputDest << "@LCL" << endl;
    this->outputDest << "M=D" << endl;

    this->outputDest << "@R7    // jump to caller return point" << endl;
    this->outputDest << "A=M" << endl;
    this->outputDest << "0; JMP" << endl;
    this->outputDest << endl;
}

/* PUBLIC METHODS */
void CodeWriter::WriteArithmetic(const std::string& command)
{
    if (!this->outputDest.is_open())
    {
        return;
    }

    if (command == "add" || command == "sub")
    {
        this->CompileSumationCommand(command);
    }
    else if (command == "neg")
    {
        this->CompileNegateCommand();
    }
    else if (command == "eq" || command == "gt" || command == "lt")
    {
        this->CompileComparisonCommand(command);
    }
    else if (command == "and" || command == "or")
    {
        this->CompileLogicalCommand(command);
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
    else if (commandType == eCommandTypePop)
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
    else
    {
        // At this point, command should be valid
        assert(false);
    }
}

void CodeWriter::WriteBranchCommand(eCommandType commandType, const string& label)
{
    if (commandType == eCommandTypeLabel)
    {
        this->CompileLabelCommand(label);
    }
    else if (commandType == eCommandTypeGoto)
    {
        this->CompileGotoCommand(label);
    }
    else if (commandType == eCommandTypeIf)
    {
        this->CompileIfCommand(label);
    }
    else
    {
        // At this poinst, command should be valid
        assert(false);
    }
}

void CodeWriter::WriteFunctionCommand(eCommandType commandType, const string& funcName, int argumentsCnt)
{
    if (commandType == eCommandTypeFunction)
    {
        this->CompileFunctionCommand(funcName, argumentsCnt);
    }
    else if (commandType == eCommandTypeCall)
    {
        this->CompileCallCommand(funcName, argumentsCnt);
    }
    else
    {
        // At this point, command should be valid
        assert(false);
    }
}

void CodeWriter::WriteReturnCommand()
{
    this->CompileReturnCommand();
}

void CodeWriter::WriteStartupCode()
{
    this->outputDest << "@256" << endl;
    this->outputDest << "D=A" << endl;
    this->outputDest << "@SP" << endl;
    this->outputDest << "M=D  // SP=256" << endl;
    this->CompileCallCommand("Sys.init", 0);
}
