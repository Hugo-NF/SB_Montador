MOD_A: BEGIN
SECTION TEXT
	MOD_B: EXTERN
	PUBLIC FAT
	PUBLIC N
	INPUT N
	LOAD N
	FAT: SUB ONE
	JMPZ FIM
	JMP MOD_B
	FIM: OUTPUT N
	STOP
SECTION BSS
	N: SPACE
SECTION DATA
	ONE: CONST 1
END