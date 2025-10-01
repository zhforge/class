        .ORIG x3000
        ;Step 1: Display the prompt
START   LEA	R0,	PROMPT
        PUTS
        LD  R1, STACK    ;R1 is the input string stack pointer
        ;Step 2: Get user input
INPUT   GETC            ;Get character input
        OUT             ;Echo to console
        ADD	R3,	R0,	#0  ;Copy input to R3
        ;Check for newline
        NOT	R4,	R3      
        ADD R4, R4, #1
        LD  R5, NL
        ADD R4, R4, R5
        BRz COMP        ;If newline, move to step 3
        ;Convert input to uppercase
        LD	R6,	CASE
        ADD R4, R3, R6  ;Add temp check value to R4
        BRn UPPER       ;to check input case
        ADD R3, R3, #-16;If lower, convert to upper
        ADD R3, R3, #-16
        ;Add input to the string stack
UPPER   STR	R3,	R1,	#0
        ADD R1, R1, #1  ;Increment stack pointer
        BR  INPUT        ;Repeat input until newline
        ;Step 3: Compare user input, output accordingly if valid
COMP    AND R4, R4, #0
        STR R4, R1, #0  ;Terminate input string with null
        ;quit
        LD  R1, STACK    ;Reset stack pointer each time
        LEA R2, QUIT
        JSR	CMPSTR
        BRn #1
        BR  STOP        ;if "quit", halt the program
        ;AABC
        LD  R1, STACK    
        LEA R2, AABC
        JSR CMPSTR
        BRn #2
        LEA R0, SCM
        BR OUTPUT
        ;AAPL
        LD  R1, STACK    
        LEA R2, AAPL
        JSR CMPSTR
        BRn #2
        LEA R0, NNM
        BR OUTPUT
        ;MSBF
        LD  R1, STACK    
        LEA R2, MSBF
        JSR CMPSTR
        BRn #2
        LEA R0, SCM
        BR OUTPUT
        ;MSFT
        LD  R1, STACK    
        LEA R2, MSFT
        JSR CMPSTR
        BRn #2
        LEA R0, NNM
        BR OUTPUT
        ;TESI
        LD  R1, STACK    
        LEA R2, TESI
        JSR CMPSTR
        BRn #2
        LEA R0, SCM
        BR OUTPUT
        ;TESS
        LD  R1, STACK    
        LEA R2, TESS
        JSR CMPSTR
        BRn #2
        LEA R0, NNM
        BR OUTPUT
        ;Step 4: Invalid input
        LEA R0, INVALID
OUTPUT  PUTS
        LEA R0, NL      ;output a newline
        PUTS
        BR START
STOP    LEA R0, LEAVE
        PUTS
        HALT
;
;Subroutine CMPSTR: compare two strings with initial pointers in R1 and R2
;uses R0-R5
;(from the project spec, need to modify)
CMPSTR  AND R0, R0, #0  ;will be modified to set condition codes as a return value
NEXT    LDR R3, R1, #0
        BRz LAST
        LDR R4, R2, #0
        NOT R4, R4
        ADD R4, R4, #1
        ADD R5, R3, R4
        BRnp DIFF
        ADD R1, R1, #1
        ADD R2, R2, #1
        BR CMPSTR
LAST    LDR R4, R2, #0
        BRnp DIFF
        ADD R0, R0, #1  ;are the same
        BR RETURN
DIFF    ADD R0, R0 #-1  ;not the same
RETURN  RET
;
NL      .STRINGZ	"\n"
INVALID .STRINGZ	"Input not valid."
LEAVE   .STRINGZ	"Thanks. See you soon."
PROMPT  .STRINGZ	"Please enter a stock symbol (quit to exit): "
QUIT    .STRINGZ	"QUIT"
AABC    .STRINGZ	"AABC"
AAPL    .STRINGZ	"AAPL"
MSBF    .STRINGZ	"MSBF"
MSFT    .STRINGZ	"MSFT"
TESI    .STRINGZ	"TESI"
TESS    .STRINGZ	"TESS"
SCM     .STRINGZ	"SCM"
NNM     .STRINGZ	"NNM"
CASE    .FILL	#-95    ;may or may not actually work, idk exactly how fill works
STACK   .FILL   x6000   ;initial input string stack address
        .END