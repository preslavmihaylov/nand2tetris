// push constant 10
@10
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// pop LCL 0
@LCL
D=M   // D=LCL
@0
D=D+A // D=D+i
@R15
M=D   // R15=D
@SP
M=M-1 // SP--
A=M
D=M   // D=(*SP)
@R15
A=M
M=D   // *(LCL+i)=D

// push constant 21
@21
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push constant 22
@22
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// pop ARG 2
@ARG
D=M   // D=ARG
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
M=D   // *(ARG+i)=D

// pop ARG 1
@ARG
D=M   // D=ARG
@1
D=D+A // D=D+i
@R15
M=D   // R15=D
@SP
M=M-1 // SP--
A=M
D=M   // D=(*SP)
@R15
A=M
M=D   // *(ARG+i)=D

// push constant 36
@36
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// pop THIS 6
@THIS
D=M   // D=THIS
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
M=D   // *(THIS+i)=D

// push constant 42
@42
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push constant 45
@45
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// pop THAT 5
@THAT
D=M   // D=THAT
@5
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

// pop THAT 2
@THAT
D=M   // D=THAT
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
M=D   // *(THAT+i)=D

// push constant 510
@510
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// pop temp 6
@R5
D=A   // D=R5
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
M=D   // *(R5+i)=D

// push LCL 0
@0
D=A   // D=i
@LCL
A=M+D
D=M   // D=*(segment+i)
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push THAT 5
@5
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

// push ARG 1
@1
D=A   // D=i
@ARG
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

// push THIS 6
@6
D=A   // D=i
@THIS
A=M+D
D=M   // D=*(segment+i)
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push THIS 6
@6
D=A   // D=i
@THIS
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

// sub
@SP
M=M-1 // SP--
A=M
D=M   // D = *SP
@SP
A=M-1
M=M-D // *(SP-1) = *(SP-1) - D

// push temp 6
@6
D=A   // D=i
@R5
A=A+D
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

