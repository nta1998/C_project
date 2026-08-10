; בדיקה בסיסית: הוראות מכל סוג, תוויות ו-.entry
; מכסה R_A_TYPE, R_C_TYPE, I_A_TYPE, I_B_TYPE, I_M_TYPE, J_TYPE

MAIN:	add $3, $5, $9
	sub $2, $1, $4
	and $8, $8, $8
	move $10, $11
	mvhi $12, $13
	addi $5, -7, $6
	ori $9, 255, $9
LOOP:	bne $7, $8, END
	beq $1, $2, LOOP
	lw $3, 16, $4
	sw $3, -8, $4
	jmp LOOP
	la END
	call MAIN
END:	hlt

.entry MAIN
.entry LOOP
