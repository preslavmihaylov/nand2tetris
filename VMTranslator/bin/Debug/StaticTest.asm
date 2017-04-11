// push constant 111
@111
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push constant 333
@333
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push constant 888
@888
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// pop static 8
@SP
M=M-1 // SP--
A=M
D=M   // D=(*SP)
@StaticTest.asm.8
M=D   // *(static+i)=D

// pop static 3
@SP
M=M-1 // SP--
A=M
D=M   // D=(*SP)
@StaticTest.asm.3
M=D   // *(static+i)=D

// pop static 1
@SP
M=M-1 // SP--
A=M
D=M   // D=(*SP)
@StaticTest.asm.1
M=D   // *(static+i)=D

// push static 3
@StaticTest.asm.3
D=M   // D = *(static+i)
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push static 1
@StaticTest.asm.1
D=M   // D = *(static+i)
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

// push static 8
@StaticTest.asm.8
D=M   // D = *(static+i)
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

