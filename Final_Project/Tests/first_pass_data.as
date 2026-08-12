
MAIN:   add  $3,$5,$9        ; IC 100
        sub  $1,$2,$3        ; IC 104
        la   BYTES           ; IC 108
        la   WORDS           ; IC 112
        la   TEXT            ; IC 116
        bgt  $4,$2,END       ; IC 120
        jmp  MAIN            ; IC 124
END:    hlt                  ; IC 128

; ============================================================

; ============================================================
BYTES:  .db 6,-9,7           ; DC 0   +3
        .db 0                ; DC 3   +1   single value
        .db 127              ; DC 4   +1   maximum
        .db -128             ; DC 5   +1   minimum
        .db 1, 2, 3          ; DC 6   +3   spaces after the commas
        .db  4 , 5 , 6       ; DC 9   +3   spaces on both sides of the comma
        .db	7,8             ; DC 12  +2   tab before the operands

; ============================================================

; ============================================================
HALVES: .dh 27056,-2         ; DC 14  +4
        .dh 32767            ; DC 18  +2   maximum
        .dh -32768           ; DC 20  +2   minimum
        .dh 0,0,0,0          ; DC 22  +8   four values

; ============================================================

; ============================================================
WORDS:  .dw 31,-12           ; DC 30  +8
        .dw 2147483647       ; DC 38  +4   maximum
        .dw -2147483648      ; DC 42  +4   minimum
        .dw 100              ; DC 46  +4

; ============================================================
; .asciz — one byte per character + terminating '\0'
; ============================================================
TEXT:   .asciz "abc"         ; DC 50  +4   3 characters + '\0'
        .asciz ""            ; DC 54  +1   empty string — only '\0'
        .asciz "a"           ; DC 55  +2   single character
        .asciz "Hello World" ; DC 57  +12  space inside the string
        .asciz "a,b,c"       ; DC 69  +6   commas inside the string (not separators!)

; ============================================================
; Directives without a label — must be handled exactly like labeled ones
; ============================================================
        .db 42               ; DC 75  +1

; ============================================================
; .entry / .extern — take no space, DC does not advance
; ============================================================
.entry BYTES
.entry K
.extern vall
.extern external_label

K:      .dw 0                ; DC 76  +4   an .entry label defined here

; ============================================================
; Expected summary:
;   ICF = 132
;   DCF = 80
;
;   Symbol table (before the step 19 relocation):
;     MAIN    100  code
;     END     128  code
;     BYTES     0  data   → after relocation 132
;     HALVES   14  data   → after relocation 146
;     WORDS    30  data   → after relocation 162
;     TEXT     50  data   → after relocation 182
;     K        80  data   → after relocation 208  (also entry)
;     vall          external
;     external_label external
; ============================================================
