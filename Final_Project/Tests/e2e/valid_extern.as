; בדיקת סמלים חיצוניים: .extern יוצר קובץ .ext עם כל מקום שימוש
; כל שימוש ב-EXT_FUNC / EXT_DATA אמור להירשם בנפרד בקובץ ה-.ext

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
