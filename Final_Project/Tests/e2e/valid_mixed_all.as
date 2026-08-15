; A macro whose body mixes instructions and a data directive, expanded
; twice, with labels defined both before and after the expansions.
; Checks that the macro expansion does not shift IC/DC and that a label
; defined after a macro call still gets the right address.

mcro setup_pair
	add $8, $9, $10
	addi $8, 5, $9
mcroend

mcro store_pair
	sw $8, 4, $9
	lw $9, -4, $8
mcroend

.entry START
.entry AFTER_MACRO
.entry TABLE
.entry TAIL

START:	setup_pair
	move $2, $3
	setup_pair

AFTER_MACRO:	store_pair
	mvhi $4, $5
	mvlo $6, $7
	nor $1, $2, $3

; Branch backwards to a label defined earlier, and forwards to one
; defined later, in the same file.
	beq $8, $9, START
	blt $1, $2, TAIL
	bgt $3, $4, AFTER_MACRO

; Jump forms: label target, register target, and hlt.
	jmp TAIL
	jmp $12
	la TABLE
	call START

TAIL:	sub $31, $0, $15
	hlt

; Data placed after all of the code, so its addresses depend on the
; final IC including both macro expansions.
TABLE:	.dw 1, -1
	.db 7
STR:	.asciz "ok"
