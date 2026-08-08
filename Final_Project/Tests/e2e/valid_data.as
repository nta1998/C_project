; בדיקת הנחיות הנתונים: .db (בית), .dh (חצי מילה), .dw (מילה), .asciz (מחרוזת)
; כולל בדיקת יישור כתובות בין הסוגים השונים

MAIN:	add $1, $2, $3
	hlt

BYTES:	.db 1, -2, 127, -128
HALFS:	.dh 300, -300, 32767
WORDS:	.dw 70000, -70000, 2147483647
STR:	.asciz "abc"
EMPTY:	.asciz ""
MIXED:	.db 5
	.dw 9

.entry BYTES
.entry STR
