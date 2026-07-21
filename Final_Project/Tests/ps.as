MAIN: add $3,$5,$9
mcro GEN_MAC
      la vall
      jmp NEXT
mcroend
LOOP: ori $9,-5,$2
GEN MAC
NEXT: move $20,$4
      bgt $4,$2,END
      la K
      SW $0,4,$10
      bne $31,$9,LOOP
      call vall
      jmp $4
END: hlt
STR: .asciz "aBcd"
LIST: .db 6,-9
      .dh 27056
.entry K
K: .dw 31,-12
.extern vall
