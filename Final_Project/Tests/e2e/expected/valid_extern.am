
.extern EXTFUNC
.extern EXTDATA

MAIN:	jmp EXTFUNC
	call EXTFUNC
	la EXTDATA
	bne $1, $2, LOCAL
LOCAL:	add $1, $2, $3
	jmp EXTFUNC
	hlt

.entry MAIN
.entry LOCAL
