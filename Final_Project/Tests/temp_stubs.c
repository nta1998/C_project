#include "globals.h"
#include "instructions.h"
#include <stdio.h>

Bool start_with_char(const char *s){
    return ((s[0]>='A'&&s[0]<='Z')||(s[0]>='a'&&s[0]<='z')) ? TRUE : FALSE;
}

Bool all_chars_valid(const char *s){
    int i;
    for (i = 0; s[i] != '\0'; i++){
        if (!((s[i]>='A'&&s[i]<='Z')||(s[i]>='a'&&s[i]<='z')||(s[i]>='0'&&s[i]<='9'))){
            return FALSE;
        }
    }
    return TRUE;
}

Bool is_reserved_word(const char *s){
    return (instruction_search(s) != NULL) ? TRUE : FALSE;
}