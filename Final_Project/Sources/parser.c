/**
 * @file parser.c
 * @brief This file contains ...
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

#include "../Headers/mcro_table.h"
#include "../Headers/parser.h"
#include "../Headers/instructions.h"
#include "../Headers/symbol_table.h"
#include "../Headers/globals.h"
#include "../Headers/errors.h"
#include "../Headers/pre_assembler.h"

/**
 * Static variable (limited to this file only):
 * @param curr_word : store the current error count.
*/
static char curr_word[MAX_LINE_LEN+1];

/**
 * 
 */
static Bool chars_are_valid(char *name){
    int i;
    for(i=0; i < strlen(name)-1; i++){
        if (!isalnum(name[i])){
            return FALSE;
        }
    }
    return TRUE;
}

static Bool is_valid_num(Line line, char *op, long min, long max){
    long val;
    int i = 0;

    if (op[0] == '+' || op[0] == '-') {
        i = 1;
    }
    
    if (op[i] == '\0') {
        err_report(line, ERR_CODE_11);
        return FALSE;
    }
    for (; op[i] != '\0'; i++) {
        if (!isdigit((unsigned char)op[i])) {
            err_report(line, ERR_CODE_12);
            return FALSE;
        }
    }
    val = atoi(op);

    if (val < min || val > max){
        err_report(line, ERR_CODE_13);
        return FALSE;
    }
    return TRUE;
}

static int nums_are_valid(char *rest, long min, long max, Line line, long values_out[]){

    char values[MAX_LINE_LEN];
    int i, j, count;
    long val;

    i = 0;
    count = 0;

    if (rest[0] == '\0'){
        err_report(line, ERR_CODE_10);
        return -1;
    }

    if (rest[0] == ','){
        err_report(line, ERR_CODE_16);
        return -1;
    }

    while (rest[i] != '\0'){
        if (i >= MAX_LINE_LEN){
            err_report(line, ERR_CODE_10);
            return -1;
        }

        j = 0;

        if(isspace(rest[i])){
            i++;
            continue;
        }
        while (rest[i] != '\0' && rest[i] != ','){
            values[j++] = rest[i++];
        }

        values[j] = '\0';

        if (!is_valid_num(line, values, min, max)){
            return -1;
        }

        val = atol(values);
        
        values_out[count] = val;
        count++;

        if (rest[i] == ','){
            i++;

            if (rest[i] == '\0'){
                err_report(line, ERR_CODE_16);
                return -1;
            }

            if (rest[i] == ','){
                err_report(line, ERR_CODE_17);
                return -1;
            }
        }
    }

    return count;
}

static Bool is_directive_word(const char *name){
    int i ;
    const char *directive_words[] = {".dh", ".dw", ".db", ".asciz", ".entry", ".extern"};
    if (name[0] != '.' ){
        return FALSE;
    }
    for (i=0 ; i < 6; i++){
        if(strcmp(name,directive_words[i]) == 0){
            return TRUE;
        }     
    }   
    return FALSE;
}

static Bool is_valid_label_syntax(const char *op){
    int i;
    if (!isalpha((unsigned char)op[0])) {
        return FALSE;
    }
    for (i = 1; op[i] != '\0'; i++) {
        if (!isalnum((unsigned char)op[i])) {
            return FALSE;
        }
    }
    return (strlen(op) <= MAX_LABEL_LEN);
}

static Bool is_valid_label_name(Line line, char *curr_word){

    if (!curr_word[0]){
        err_report(line, ERR_CODE_21);
        return FALSE;
    }
    if (!chars_are_valid(curr_word)){
        err_report(line, ERR_CODE_22);
        return FALSE;
    } 
    if (strlen(curr_word) > MAX_LABEL_LEN) {
        err_report(line, ERR_CODE_23);
        return FALSE;
    }
    if (is_instruction_word(curr_word)){
        err_report(line, ERR_CODE_24);
        return FALSE;
    } 
    if (mcro_search(curr_word) != NULL){
        err_report(line, ERR_CODE_25);
        return FALSE;
    } 
    if (is_reserved_word(curr_word)){
        err_report(line, ERR_CODE_26);
        return FALSE;
    } 
    if (curr_word[strlen(curr_word)-1] != ':'){
        err_report(line, ERR_CODE_28);
        return FALSE;
    }
    if (isspace(curr_word[strlen(curr_word)-2])){
        err_report(line, ERR_CODE_27);
        return FALSE;
    }
    curr_word[strlen(curr_word)-1]='\0';
    return TRUE;
}

static Bool is_valid_reg(Line line, char *op){
    int i,val;
    if (op[0] != '$' || op[1] == '\0'){
        err_report(line, ERR_CODE_11);
        return FALSE;
    }
    for (i = 1; op[i] != '\0'; i++) {
        if (!isdigit((unsigned char)op[i])) {
            err_report(line, ERR_CODE_11);
            return FALSE;
        }
    }
    val = atoi(op + 1);
    if (val < FIRST_REG || val > LAST_REG){
        err_report(line, ERR_CODE_14);
        return FALSE;
    }
    return TRUE;
}

static Bool is_valid_instruction(char *op1, char *op2, char *op3, Parsed_line *curr_line, int *count, Line line){

    if (curr_line->instruction->type == R_A_TYPE){
        if (*count != 2){
            err_report(line, ERR_CODE_10);
            return FALSE;
        }
        if (!is_valid_reg(line, op1) || !is_valid_reg(line, op2) || !is_valid_reg(line, op3)){
            return FALSE;
        }
    }

    if (curr_line->instruction->type == R_C_TYPE){
        if (*count != 1){
            err_report(line, ERR_CODE_10);
            return FALSE;
        }
        if (!is_valid_reg(line, op1) || !is_valid_reg(line, op2)){
            return FALSE;
        }

    }

    if (curr_line->instruction->type == I_B_TYPE){
        if (*count != 2){
            err_report(line, ERR_CODE_10);
            return FALSE;
        }
        if (!is_valid_reg(line, op1) || !is_valid_reg(line, op2) || !is_valid_label_syntax(op3)){
            if(!is_valid_label_syntax(op3)){
            err_report(line, ERR_CODE_30);
            }
            return FALSE;
        }
    }

    if ((curr_line->instruction->type == I_A_TYPE) || (curr_line->instruction->type == I_M_TYPE)){
        if (*count != 2){
            err_report(line, ERR_CODE_10);
            return FALSE;
        }
        if (!is_valid_reg(line, op1) || !is_valid_num(line, op2, IMMEDIATE_MIN, IMMEDIATE_MAX) || !is_valid_reg(line, op3) ){
            return FALSE;
        }
    }

    if (curr_line->instruction->type == J_TYPE){
        if (strcmp(curr_line->name, "hlt") == 0){
            if (*count != 0){
                err_report(line, ERR_CODE_10);
                return FALSE;
            }
        }
        else {
            if (*count != 0){
                err_report(line, ERR_CODE_10);
                return FALSE;
            }
            if (strcmp(curr_line->name, "la") == 0 || strcmp(curr_line->name, "call") == 0){
                if (op1[0] == '$'){
                    err_report(line, ERR_CODE_11);
                    return FALSE;
                }
                if (!is_valid_label_syntax(op1)){
                    err_report(line, ERR_CODE_30);
                    return FALSE;
                }
            }
            else {
                if (op1[0] == '$'){
                    if(!is_valid_reg(line, op1)){
                        return FALSE;
                    }
                }
                else {
                    if (!is_valid_label_syntax(op1)){
                        err_report(line, ERR_CODE_30);
                        return FALSE;
                    }
                }
            }
        }

    }
    return TRUE;
}

static Bool split_instruction_operands(Line line, Parsed_line *curr_line, char ops[MAX_OPERANDS][MAX_OPERAND_LEN], int *count_out){

    int i,j;
    Bool is_comma;

    if (curr_line->rest[0] == ','){
        err_report(line, ERR_CODE_16);
        return FALSE;
    } 

    is_comma = FALSE;
    j = 0;
    
    for (i = 0; curr_line->rest[i] != '\0'; i++){
        if (curr_line->rest[i] == ',') {
            if (is_comma) {
                err_report(line, ERR_CODE_17);
                return FALSE;
            }
            if (curr_line->rest[i+1] == '\0'){
                err_report(line, ERR_CODE_16);
                return FALSE;
            }
            ops[*count_out][j] = '\0';
            (*count_out)++;
            is_comma = TRUE;
            j = 0;
        }
        else if(isspace(curr_line->rest[i])){
            continue;
        }
        else {
            is_comma = FALSE;
            ops[*count_out][j] = curr_line->rest[i];
            j++;
        }
    }
    ops[*count_out][j] = '\0';

    return is_valid_instruction(ops[0],ops[1],ops[2], curr_line, count_out, line);
}

static Bool split_directive_operands(Line line, Parsed_line *curr_line, long values_out[], int *count_out){

    if (strcmp(curr_line->name, ".asciz") == 0){
        if (curr_line->rest[0] != '"'){
            err_report(line, ERR_CODE_18);
            return FALSE;
        }

        if (curr_line->rest[strlen(curr_line->rest)-1] != '"'){
            err_report(line, ERR_CODE_19);
            return FALSE;
        }

        return TRUE;
    }
    if (strcmp(curr_line->name, ".db") == 0){
        *count_out = nums_are_valid(curr_line->rest, MIN_DB_OP, MAX_DB_OP, line, values_out);
        return (*count_out != -1);    
    }
    if (strcmp(curr_line->name, ".dh") == 0){
        *count_out = nums_are_valid(curr_line->rest, MIN_DH_OP, MAX_DH_OP, line, values_out);
        return (*count_out != -1);    
    }
    if (strcmp(curr_line->name, ".dw") == 0){
        *count_out = nums_are_valid(curr_line->rest, MIN_DW_OP, MAX_DW_OP, line, values_out);
        return (*count_out != -1);    
    }
    
    return TRUE;

}

Bool is_valid_rest_split(Line line, Parsed_line *curr_line, char ops[MAX_OPERANDS][MAX_OPERAND_LEN], long values_out[], int *count_out){

    Bool operands_ok;

    if (curr_line->kind == LINE_DIRECTIVE) {
        operands_ok = split_directive_operands(line, curr_line, values_out, count_out);
    }

    if (curr_line->kind == LINE_INSTRUCTION) {
        operands_ok = split_instruction_operands(line, curr_line, ops, count_out);
    }

    if (curr_line->label[0] != '\0') {
        return (operands_ok && is_valid_label_name(line, curr_line->label));
    }

    return operands_ok;
}

void skip_white_space(int *start_index, Line line){
    while (isspace(line.data[*start_index])) {
        (*start_index)++;}
}

void find_next_word(int *start_index, Line line){
    int i=0;
    while (line.data[*start_index] != '\0' && !isspace(line.data[*start_index])) {
        curr_word[i] =line.data[*start_index];
        (*start_index)++;
        i++;
    }
    curr_word[i] = '\0';
}

Bool split_line(Line line, Parsed_line *curr_line){
    const Instruction_info *curr_info;
    int start_index, j;
    start_index = 0;
    j = 0;

    curr_line->label[0] = '\0';
    curr_line->name[0] = '\0';
    curr_line->rest[0] = '\0';

    skip_white_space(&start_index, line);

    if (line.data[start_index] == '\0') {
        curr_line->kind = LINE_EMPTY;
        return TRUE;
    }

    if (line.data[start_index] == ';') {
        curr_line->kind = LINE_COMMENT;
        return TRUE;
    }

    if (strlen(line.data) > MAX_LINE_LEN){
        err_report(line, ERR_CODE_9);
        return FALSE;
    }

    while (line.data[start_index] != '\0'){
        j=0;
        find_next_word(&start_index, line);
        curr_info = instruction_search(curr_word);

        if (is_directive_word(curr_word)){
            curr_line->kind = LINE_DIRECTIVE;
            strcpy(curr_line->name, curr_word);
        }
        else if (curr_info != NULL){
            curr_line->kind = LINE_INSTRUCTION;
            curr_line->instruction = curr_info;
            strcpy(curr_line->name, curr_word);
        }

        else{
            strcpy(curr_line->label, curr_word);
            skip_white_space(&start_index, line);
            continue;
        }

        skip_white_space(&start_index, line);
        while (line.data[start_index] != '\0'){
            if (!isspace(line.data[start_index])){
                curr_line->rest[j] = line.data[start_index];
                j++;
            }
            start_index++;
        }
        curr_line->rest[j] = '\0';
    }


    return TRUE;
}