// push constant 17
@17
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push constant 17
@17
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// eq
@SP
M=M-1 // SP--
A=M
D=M   // D = *SP
@SP
A=M-1
D=M-D // D = *(SP-1) - D
@StackTest.asm.TRUE.0
D; JEQ // if (D == 0) goto TRUE
@SP
A=M-1
M=0   // *(SP-1) = false
@StackTest.asm.END.0
0; JMP // goto END
(StackTest.asm.TRUE.0)
@SP
A=M-1
M=-1   // *(SP-1) = true
(StackTest.asm.END.0)

// push constant 17
@17
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push constant 16
@16
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// eq
@SP
M=M-1 // SP--
A=M
D=M   // D = *SP
@SP
A=M-1
D=M-D // D = *(SP-1) - D
@StackTest.asm.TRUE.1
D; JEQ // if (D == 0) goto TRUE
@SP
A=M-1
M=0   // *(SP-1) = false
@StackTest.asm.END.1
0; JMP // goto END
(StackTest.asm.TRUE.1)
@SP
A=M-1
M=-1   // *(SP-1) = true
(StackTest.asm.END.1)

// push constant 16
@16
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push constant 17
@17
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// eq
@SP
M=M-1 // SP--
A=M
D=M   // D = *SP
@SP
A=M-1
D=M-D // D = *(SP-1) - D
@StackTest.asm.TRUE.2
D; JEQ // if (D == 0) goto TRUE
@SP
A=M-1
M=0   // *(SP-1) = false
@StackTest.asm.END.2
0; JMP // goto END
(StackTest.asm.TRUE.2)
@SP
A=M-1
M=-1   // *(SP-1) = true
(StackTest.asm.END.2)

// push constant 892
@892
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push constant 891
@891
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// lt
@SP
M=M-1 // SP--
A=M
D=M   // D = *SP
@SP
A=M-1
D=M-D // D = *(SP-1) - D
@StackTest.asm.TRUE.3
D; JLT // if (D < 0) goto TRUE
@SP
A=M-1
M=0   // *(SP-1) = false
@StackTest.asm.END.3
0; JMP // goto END
(StackTest.asm.TRUE.3)
@SP
A=M-1
M=-1   // *(SP-1) = true
(StackTest.asm.END.3)

// push constant 891
@891
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push constant 892
@892
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// lt
@SP
M=M-1 // SP--
A=M
D=M   // D = *SP
@SP
A=M-1
D=M-D // D = *(SP-1) - D
@StackTest.asm.TRUE.4
D; JLT // if (D < 0) goto TRUE
@SP
A=M-1
M=0   // *(SP-1) = false
@StackTest.asm.END.4
0; JMP // goto END
(StackTest.asm.TRUE.4)
@SP
A=M-1
M=-1   // *(SP-1) = true
(StackTest.asm.END.4)

// push constant 891
@891
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push constant 891
@891
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// lt
@SP
M=M-1 // SP--
A=M
D=M   // D = *SP
@SP
A=M-1
D=M-D // D = *(SP-1) - D
@StackTest.asm.TRUE.5
D; JLT // if (D < 0) goto TRUE
@SP
A=M-1
M=0   // *(SP-1) = false
@StackTest.asm.END.5
0; JMP // goto END
(StackTest.asm.TRUE.5)
@SP
A=M-1
M=-1   // *(SP-1) = true
(StackTest.asm.END.5)

// push constant 32767
@32767
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push constant 32766
@32766
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// gt
@SP
M=M-1 // SP--
A=M
D=M   // D = *SP
@SP
A=M-1
D=M-D // D = *(SP-1) - D
@StackTest.asm.TRUE.6
D; JGT // if (D > 0) goto TRUE
@SP
A=M-1
M=0   // *(SP-1) = false
@StackTest.asm.END.6
0; JMP // goto END
(StackTest.asm.TRUE.6)
@SP
A=M-1
M=-1   // *(SP-1) = true
(StackTest.asm.END.6)

// push constant 32766
@32766
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push constant 32767
@32767
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// gt
@SP
M=M-1 // SP--
A=M
D=M   // D = *SP
@SP
A=M-1
D=M-D // D = *(SP-1) - D
@StackTest.asm.TRUE.7
D; JGT // if (D > 0) goto TRUE
@SP
A=M-1
M=0   // *(SP-1) = false
@StackTest.asm.END.7
0; JMP // goto END
(StackTest.asm.TRUE.7)
@SP
A=M-1
M=-1   // *(SP-1) = true
(StackTest.asm.END.7)

// push constant 32766
@32766
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push constant 32766
@32766
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// gt
@SP
M=M-1 // SP--
A=M
D=M   // D = *SP
@SP
A=M-1
D=M-D // D = *(SP-1) - D
@StackTest.asm.TRUE.8
D; JGT // if (D > 0) goto TRUE
@SP
A=M-1
M=0   // *(SP-1) = false
@StackTest.asm.END.8
0; JMP // goto END
(StackTest.asm.TRUE.8)
@SP
A=M-1
M=-1   // *(SP-1) = true
(StackTest.asm.END.8)

// push constant 57
@57
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push constant 31
@31
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// push constant 53
@53
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

// push constant 112
@112
D=A   // D=i
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

// neg
@SP
A=M-1
M=-M  // (*(SP-1)) = -(*(SP-1))

// and
@SP
M=M-1 // SP--
A=M
D=M   // D = *SP
@SP
A=M-1
M=D&M // *(SP-1) = D&(*(SP-1))

// push constant 82
@82
D=A   // D=i
@SP
M=M+1 // SP++
A=M-1
M=D   // *(SP-1)=D

// or
@SP
M=M-1 // SP--
A=M
D=M   // D = *SP
@SP
A=M-1
M=D|M // *(SP-1) = D|(*(SP-1))

// not
@SP
A=M-1
M=!M  // *(SP-1)=!(*(SP-1))

