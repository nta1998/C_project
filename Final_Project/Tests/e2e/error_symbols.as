; שגיאות בשלב המעבר השני (ERR_CODE_30 עד ERR_CODE_33)
; אף קובץ פלט לא אמור להיווצר

.extern EXT_SYM
.extern BOTH_SYM

MAIN:	jmp UNDEFINED_LABEL
	la ALSO_UNDEFINED
	bne $1, $2, EXT_SYM
	add $1, $2, $3
	hlt

.entry BOTH_SYM
.entry NOT_DEFINED_ANYWHERE
