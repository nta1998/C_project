MAIN:   add  $3,$5,$9        
        sub  $1,$2,$3        ; 104  R_A
        and  $9,$8,$7        ; 108  R_A
LOOP:   ori  $9,-5,$2        ; 112  I_A
        addi $4,32767,$5     ; 116  I_A  immediate מקסימלי
        subi $4,-32768,$5    ; 120  I_A  immediate מינימלי
        move $20,$4          ; 124  R_C  2 registers
        mvhi $1,$2           ; 128  R_C
        bgt  $4,$2,END       ; 132  I_B  קפיצה קדימה (יושלם ב-second pass)
        bne  $31,$9,LOOP     ; 136  I_B  קפיצה אחורה
        sw   $0,4,$10        ; 140  I_M
        lb   $3,-28,$18      ; 144  I_M
        la   K               ; 148  J    תווית של data
        call vall            ; 152  J    סמל חיצוני
        jmp  $4              ; 156  J    צורת רגיסטר
        jmp  END             ; 160  J    צורת תווית
END:    hlt                  ; 164  J    ללא אופרנדים

; --- מקטע הנתונים: DC מתחיל ב-0 ומוזז ב-ICF בסוף המעבר (step 19) ---
STR:    .asciz "abc"         ; DC 0..3   (3 תווים + '\0')
LIST:   .db 6,-9,7           ; DC 4..6   (בית לכל ערך)
HALF:   .dh 27056,-2         ; DC 7..10  (2 בתים לכל ערך)
K:      .dw 31,-12           ; DC 11..18 (4 בתים לכל ערך)

.entry K
.extern vall
