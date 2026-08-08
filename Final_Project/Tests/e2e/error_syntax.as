; שגיאות תחביר בשלב המעבר הראשון (ERR_CODE_10 עד ERR_CODE_29)
; אף קובץ פלט לא אמור להיווצר

MAIN:	add $1, $2
	add $1, $2, $3, $4
	nosuchop $1, $2, $3
	add $1, $2, $99
	add $1, $2, $notareg
	addi $1, 99999999999, $2
	add $1,, $2
	add , $1, $2
	add $1, $2, $3,
9BAD:	hlt
bad$label:	hlt
add:	hlt
MISSING	hlt
SPACED :	hlt
MAIN:	hlt
	.asciz missing_quotes
	.asciz "unterminated
