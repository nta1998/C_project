; Operand-level errors in the first pass: bad registers, wrong operand
; counts, comma placement, out-of-range immediates and bad label names.
; Every line below is deliberately wrong; none of them should assemble.

; Register numbers outside $0-$31.
MAIN:	add $32, $1, $2
	sub $1, $99, $2
	move $-1, $4

; Wrong number of operands.
	add $1, $2
	hlt $1
	move $1, $2, $3

; Comma problems.
	add $1 $2, $3
	sub $1,, $2, $3
	and $1, $2, $3,

; Immediate values outside the signed 16-bit range.
	addi $1, 40000, $2
	subi $1, -40000, $2

; Numeric directive values outside the range of their size.
	.db 128
	.db -129
	.dh 40000
	.dh -40000

; Label problems.
1BAD:	add $1, $2, $3
BAD@NAME:	add $1, $2, $3
add:	add $1, $2, $3

; Unknown instruction.
	notreal $1, $2, $3

; Unterminated string.
STR:	.asciz "no closing quote
