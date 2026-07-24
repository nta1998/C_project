.
add $3, $19, $8
   add    $3 ,$19,   $8
LOOP: add $1,$2,$3
hlt
END: hlt
jmp $7
addi $9, -45, $8
sw $7, -28, $18
STR: .asciz "hello"
ARR: .dh 5, -1, 130
.entry MAIN
.extern val1

; שורת הערה שלמה - אמורה להיות COMMENT
    ; גם זו הערה עם רווחים לפני

; --- שורות ריקות (EMPTY) ---

	

; --- שורות שאמורות להידחות (FALSE) ---
1abc: add $1,$2,$3
LOOP : add $1,$2,$3
add: hlt
LOOP:
$1, $2
add $1,,$2
add $1, $2,
,$1
averyveryveryveryveryverylonglabelover31: hlt