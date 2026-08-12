/**
 * @file pre_assembler.c
 * @brief This file role is to perform the pre-assembly stage of the assembler.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../Headers/pre_assembler.h"
#include "../Headers/parser.h"
#include "../Headers/instructions.h"
#include "../Headers/globals.h"

/* Possible classifications for a line when we are NOT currently inside a macro definition (curr_mcro == NULL). */
typedef enum {
    MCRO_START,
    MCRO_CALL,
    REGULAR
} MCRO_OFF;

/* Possible classifications for a line when we ARE currently inside a macro definition (curr_mcro != NULL). */
typedef enum {
    MCRO_BODY,
    MCRO_END
} MCRO_ON;

/**
 * Static function that called only if the flag `curr_mcro` == `NULL`,
 * and its role is to return the type of row involved : MCRO_START, MCRO_CALL or MCRO_END.
 * @param line: pointer to the current line.
 * @return an MCRO_OFF indicating the type of the line (MCRO_START, MCRO_CALL, or REGULAR).
*/
static MCRO_OFF mcro_off(const char *line){
    char first_word[MAX_LINE_LEN+1];
    sscanf(line, "%s", first_word);
    if (strcmp(first_word, "mcro") == 0){
        return MCRO_START;
    }
    if (mcro_search(first_word)){
        return MCRO_CALL;
    }
    return REGULAR;
}

/**
 * Static function that called only if the flag `curr_mcro` != `NULL`,
 * and its role is to return the type of row involved : MCRO_BODY or MCRO_END.
 * @param line: pointer to the current line.
 * @return an MCRO_ON indicating the type of the line (MCRO_BODY or MCRO_END).
*/
static MCRO_ON mcro_on(const char *line){
    char first_word[MAX_MCRO_LEN+1];
    sscanf(line, "%s", first_word); 
    if (strcmp(first_word, "mcroend") == 0){
        return MCRO_END;
    }
    return MCRO_BODY;
}

/**
 * Static function that checks whether the current line is a valid macro definition line.
 * @param curr_line_data: pointer to the current line data.
 * @return a Bool indicating whether the line is valid (TRUE) or not (FALSE).
*/
static Bool valid_mcro_def_line(const char *curr_line_data){
    char extra[MAX_LINE_LEN+1];
    if (sscanf(curr_line_data, "%*s %*s %s", extra) == 1){
        return FALSE;
    }
    return TRUE;
}

/**
 * Static function that checks whether the current line is a valid macroend definition line.
 * @param curr_line_data: pointer to the current line data.
 * @return a Bool indicating whether the line is valid (TRUE) or not (FALSE).
*/
static Bool valid_mcroend_def_line(const char *curr_line_data){
    char extra[MAX_LINE_LEN+1];
    if (sscanf(curr_line_data, "%*s %s", extra) == 1){
        return FALSE;
    }
    return TRUE;
}

/**
 * Static function that checks whether the current mcro name contains letters/numbers/'_' only.
 * @param name: pointer to the current mcro name.
 * @return a Bool indicating whether the mcro name is valid (TRUE) or not (FALSE).
*/
static Bool all_chars_valid(char *name){
    int i = 0;
    while (mcro_name[i] != '\0'){
        if (!isalnum(mcro_name[i]) && mcro_name[i] != '_'){
            return FALSE;
        }
        i++;
    }

    if (strspn(name, allowed) != strlen(name)){
        return FALSE;
    }
    return TRUE;
}

/**
 * Static function that checks whether the current mcro name is within the specified maximum length.
 * @param name: pointer to the current mcro name.
 * @return a Bool indicating whether the mcro name is valid (TRUE) or not (FALSE).
*/
static Bool is_len_valid(char *name){
    if (strlen(name) > MAX_MCRO_LEN) {
        return FALSE;
    }
    return TRUE;
}

/**
 * Static function that checks whether the current mcro name is instruction word.
 * @param name: pointer to the current mcro name.
 * @return a Bool indicating whether the mcro name is valid (TRUE) or not (FALSE).
*/
static Bool is_instruction_word(const char *name){
    if (instruction_search(name) != NULL) {
        return TRUE;
    }
    return FALSE;
}

Bool is_reserved_word(const char *name){
    int i, num_keywords;
    const char *keywords[] = {
        "db", "dw", "dh", "asciz", "entry", "extern",
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
    };
    num_keywords = sizeof(keywords) / sizeof(keywords[0]);

    for (i = 0; i < num_keywords; i++) {
        if (strcmp(name, keywords[i]) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * Static function that expands a macro call by writing the corresponding macro lines to the output file.
 * @param line: pointer to the current line containing the macro call.
 * @param curr_am_file: pointer to the output file where the expanded macro lines will be written.
*/
static void expand_mcro_call(const char *line, FILE *curr_am_file){
    char first_word[MAX_MCRO_LEN+1];
    Mcro *found;
    int i;
    sscanf(line, "%s", first_word);
    found = mcro_search(first_word);
    for (i=0; i < found -> line_counter; i++){
        fputs(found->data[i], curr_am_file);
    }
}

/**
 * Static function that checks whether the current line is a valid macro definition line.
 * @param curr_line: the current line's context (passed by value).
 * @return Mcro structure if the line is valid, or NULL if it is not.
*/
static Mcro *start_mcro_def(Line curr_line){
    char mcro_name[MAX_MCRO_LEN+1];
    sscanf(curr_line.data, "%*s %s", mcro_name);

    if (!valid_mcro_def_line(curr_line.data)){
        err_report(curr_line, ERR_CODE_1);
        return NULL;
    } 
    if (is_instruction_word(mcro_name)){
        err_report(curr_line, ERR_CODE_3);
        return NULL;
    } 
    if (is_reserved_word(mcro_name)){
        err_report(curr_line, ERR_CODE_4);
        return NULL;
    } 
    if (!isalpha(mcro_name[0])){
        err_report(curr_line, ERR_CODE_5);
        return NULL;
    } 
    if (!all_chars_valid(mcro_name)){
        err_report(curr_line, ERR_CODE_6);
        return NULL;
    } 
    if (!is_len_valid(mcro_name)){
        err_report(curr_line, ERR_CODE_7);
        return NULL;
    } 
    if (mcro_search(mcro_name) != NULL){
        err_report(curr_line, ERR_CODE_8);
        return NULL;
    } 
    return mcro_add(mcro_name);
}

Bool pre_assembler(const char *as_file, const char *am_file){
    FILE *origin_file, *new_file;
    Mcro *curr_mcro = NULL;
    Line curr_line;
    char line[MAX_LINE_LEN + 1];

    curr_line.file_name = as_file;
    curr_line.line_num = 0;

    origin_file = fopen(as_file, "r");
    new_file = fopen(am_file, "w");

    if(new_file == NULL){
        fclose(origin_file);
        return FALSE;
    }


    while (fgets(line,MAX_LINE_LEN, origin_file) != NULL){
        curr_line.line_num ++;
        curr_line.data = line;
        if(curr_mcro == NULL){
            switch (mcro_off(line)) {
                case MCRO_CALL:
                    expand_mcro_call(line, new_file);
                    break;
                case MCRO_START:
                    curr_mcro = start_mcro_def(curr_line);
                    break;
                case REGULAR:
                    fputs(line, new_file);
                    break;
            }
        }
        else{
            switch (mcro_on(line)){
                case MCRO_END: {
                    if (!valid_mcroend_def_line(curr_line.data)){
                        err_report(curr_line, ERR_CODE_2);
                    }
                    curr_mcro = NULL;
                    break;}
                case MCRO_BODY:
                    mcro_add_line(curr_mcro, line);
                    break;
            }

        }
    }

    fclose(origin_file);
    fclose(new_file);

    if (err_found()){
        remove(am_file);
        return FALSE;
    }

    return TRUE;
}
