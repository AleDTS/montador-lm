INICIO  EQU 100
FIM     EQU 103
SOMA    EQU 241
CONT    EQU 242
MEDIA   EQU 240
        MOV A,0;	[0]
        MOV [CONT],A	
        MOV [MEDIA],A;	[4]
        MOV A,[INICIO];	[6]
        MOV [SOMA],A;	[8]
        MOV A,INICIO;	[10]
        MOV B,A;	[12]
LACOS   MOV A,[SOMA];	[13]
        INC B;		[15]
        ADD A,[B];	[16]
        MOV [SOMA],A;	[17]
        MOV A,[CONT];	[19]
        INC A;		[21]
        MOV [CONT],A;	[22]
        MOV A,B;	[24]
        CMP A,FIM;	[25]
        JBE LACOS;	[27]
LACOM   MOV A,[SOMA];	[29]
        SUB A,[CONT];	[31]
        JC ACABA;	[33]
        MOV [SOMA],A;	[35]
        MOV A,[MEDIA];	[37]
        INC A;		[39]
        MOV [MEDIA],A;	[40]
        JMP LACOM;	[42]
ACABA   HLT ;		[44] 
