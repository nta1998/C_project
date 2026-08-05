#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../Headers/instructions.h"
#include "../Headers/globals.h"
#include "../Headers/symbol_table.h"

 typedef enum {
    EMPTY_LINE,
    COMMENT_LINE,
    INSTRUCTION_LINE,
    LABLE_LINE,
    DIRECTIVE_LINE,
    LABLE_AND_DIRECTIVE_LINE,
    INVALID_LINE
 } LINE_TYPE;

 LINE_TYPE line_type(const char *line){
    int i;
    const char *reserved_words[] = {".dh", ".dw", ".db", ".asciz", ".entry", ".extern"};
    char first_word[MAX_LINE_LEN+1];
    char second_word[MAX_LINE_LEN+1];
    sscanf(line, "%s", first_word);

    if (strcmp(first_word, "\0") == 0) {
        return EMPTY_LINE;
    }
    if (strcmp(first_word, ";") == 0) {
        return COMMENT_LINE;
    }
    if (instruction_search(first_word) != NULL) {
        return INSTRUCTION_LINE;
    }
    if (strcmp(first_word[-2], " ") != 0 && strcmp(first_word[-1], ":") == 0) {
        sscanf(line, "%*s %s", second_word);
        for (i=0; i<sizeof(reserved_words); i++){
            if(strcmp(second_word,reserved_words[i]) == 0){
                return LABLE_AND_DIRECTIVE_LINE;
            } 
        }
        return LABLE_LINE;}
    for (i=0; i<sizeof(reserved_words); i++){
        if(strcmp(first_word,reserved_words[i]) == 0){
            return DIRECTIVE_LINE;
        } 
    }
    return INVALID_LINE;
 }









