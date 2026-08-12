
.extern EXT_FUNC
.extern EXT_DATA

MAIN:	jmp EXT_FUNC
	call EXT_FUNC
	la EXT_DATA
	bne $1, $2, LOCAL
LOCAL:	add $1, $2, $3
	jmp EXT_FUNC
	hlt

.entry MAIN
.entry LOCAL
