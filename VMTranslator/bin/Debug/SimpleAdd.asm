// push constant 7
@7
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push constant 8
@8
D=A   // D=i
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

