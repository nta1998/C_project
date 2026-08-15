; Boundary values on every field that has a documented range.
; Registers $0 and $31, immediate values at both ends of the 16-bit
; signed range, and .db/.dh/.dw values at their minimum and maximum.
; Also checks byte packing when data sizes do not divide evenly by 4.

.entry EDGES
.entry MINMAX
.entry ODD

EDGES:	add $0, $31, $0
	sub $31, $31, $31
	move $0, $31
	mvhi $31, $0

; Immediate at the extremes of the signed 16-bit field.
	addi $1, 32767, $2
	subi $1, -32768, $2
	andi $0, 0, $31
	ori $31, -1, $0
	nori $5, 1, $6

; Memory instructions with negative and positive offsets.
	lb $1, -32768, $2
	sb $3, 32767, $4
	lh $5, -1, $6
	sh $7, 0, $8

	hlt

; Numeric directives at their exact limits.
MINMAX:	.db -128, 127
	.dh -32768, 32767
	.dw -2147483648, 2147483647

; A single byte followed by a word, so the word does not start on a
; 4-byte boundary and the object rows must repack across entries.
ODD:	.db 1
	.dw 2
	.db 3
	.dh 4
	.asciz "z"
