; שגיאות בשלב הפרה-אסמבלר (ERR_CODE_1 עד ERR_CODE_8)
; אף קובץ פלט לא אמור להיווצר

mcro add
	hlt
mcroend

mcro .entry
	hlt
mcroend

mcro 9bad
	hlt
mcroend

mcro bad$name
	hlt
mcroend

mcro good_one extra_text_here
	hlt
mcroend

mcro closing_extra
	hlt
mcroend trailing_text

mcro dup_name
	hlt
mcroend

mcro dup_name
	hlt
mcroend

MAIN:	hlt
