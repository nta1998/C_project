MAIN:   add  $3,$5,$9        
        sub  $1,$2,$3        ; 104  R_A
        and  $9,$8,$7        ; 108  R_A
LOOP:   ori  $9,-5,$2        ; 112  I_A
        addi $4,32767,$5     ; 116  I_A  immediate מקסימלי
        subi $4,-32768,$5    ; 120  I_A  immediate מינימלי
        move $20,$4          ; 124  R_C  2 registers
        mvhi $1,$2           ; 128  R_C
        bgt  $4,$2,END       ; 132  I_B  קפיצה קדימה 
        bne  $31,$9,LOOP     ; 136  I_B  קפיצה אחורה
        sw   $0,4,$10        ; 140  I_M
        lb   $3,-28,$18      ; 144  I_M
        la   K               ; 148  J    תווית של data
        call vall            ; 152  J    סמל חיצוני
        jmp  $4              ; 156  J    צורת רגיסטר
        jmp  END             ; 160  J    צורת תווית
END:    hlt                  ; 164  J    ללא אופרנדים

; --- מקטע הנתונים: DC מתחיל ב-0 ומוזז ב-ICF
STR:    .asciz "abc"        
LIST:   .db 6,-9,7          
HALF:   .dh 27056,-2         
K:      .dw 31,-12          

.entry K
.extern vall
