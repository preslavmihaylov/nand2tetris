// push constant 3030
@3030
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// pop pointer 0
@SP
M=M-1 // SP--
A=M
D=M  // D=(*SP)
@THIS
M=D  // THIS=D

// push constant 3040
@3040
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// pop pointer 1
@SP
M=M-1 // SP--
A=M
D=M  // D=(*SP)
@THAT
M=D  // THAT=D

// push constant 32
@32
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// pop THIS 2
@THIS
D=M   // D=THIS
@2
D=D+A // D=D+i
@R15
M=D   // R15=D
@SP
M=M-1 // SP--
A=M
D=M   // D=(*SP)
@R15
A=M
M=D   // *(THIS+i)=D

// push constant 46
@46
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// pop THAT 6
@THAT
D=M   // D=THAT
@6
D=D+A // D=D+i
@R15
M=D   // R15=D
@SP
M=M-1 // SP--
A=M
D=M   // D=(*SP)
@R15
A=M
M=D   // *(THAT+i)=D

// push pointer 0
@THIS
D=M   // D=THIS
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push pointer 1
@THAT
D=M   // D=THAT
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// add
@SP
M=M-1 // SP--
A=M
D=M   // D = (*SP)
@SP
A=M-1
M=D+M // *(SP-1) = D + *(SP-1)

// push THIS 2
@2
D=A   // D=i
@THIS
A=M+D
D=M   // D=*(segment+i)
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// sub
@SP
M=M-1 // SP--
A=M
D=M   // D = *SP
@SP
A=M-1
M=M-D // *(SP-1) = *(SP-1) - D

// push THAT 6
@6
D=A   // D=i
@THAT
A=M+D
D=M   // D=*(segment+i)
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// add
@SP
M=M-1 // SP--
A=M
D=M   // D = (*SP)
@SP
A=M-1
M=D+M // *(SP-1) = D + *(SP-1)

