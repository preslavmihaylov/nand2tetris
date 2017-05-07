// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, the
// program clears the screen, i.e. writes "white" in every pixel.

// Put your code here.

(LOOP)
	@i
	M=0
	
	// if no input --> white screen
	@KBD
	D=M
	@WHITE_SCR_LOOP
	D; JEQ
	
	// else --> black screen
	@BLACK_SCR_LOOP
	0; JEQ
	
		// Turn screen white
		(WHITE_SCR_LOOP)
		// set current byte of screen to black
		@i
		D=M

		@SCREEN
		A=A+D
		M=0

		@i
		M=M+1

		@KBD
		D=A

		@i
		D=D-M

		@SCREEN
		D=D-A

		@LOOP
		D;JEQ
		
		@WHITE_SCR_LOOP
		0; JEQ
		
		//	Turn screen black
		(BLACK_SCR_LOOP)
		@i
		D=M

		@SCREEN
		A=A+D
		M=-1

		@i
		M=M+1

		@KBD
		D=A

		@i
		D=D-M

		@SCREEN
		D=D-A

		@LOOP
		D;JEQ
		
		@BLACK_SCR_LOOP
		0; JEQ
	
	@LOOP
	0; JEQ

(END)
	@END
	0;JEQ