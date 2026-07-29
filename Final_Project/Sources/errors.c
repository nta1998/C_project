/**
 * @file errors.c
 * @brief This file contains the implementation of error handling functions for the assembler.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../Headers/errors.h"

/* Static variable (limited to this file only) – store the current error count. */
static int err_count;

/* The error codes are used to identify and report errors encountered during the assembly process.*/
static Error errors[] = {
    {CODE_0, "NO ERRORS FOUND!"},
    {ERR_CODE_1, "Extra text on the same line before or after mcro name definition."},
    {ERR_CODE_2, "Extra text on the same line after mcroend."},
    {ERR_CODE_3, "Invalid mcro name, identical to an instruction name."},
    {ERR_CODE_4, "Invalid mcro name, does not start with a letter."},
    {ERR_CODE_5, "Invalid mcro name, there is an unauthorized character."},
    {ERR_CODE_6, "Invalid mcro name, longer than 31 characters."},
    {ERR_CODE_7, "Mcro definition repeats more then once."},
    {ERR_CODE_8, "Invalid line length, it is longer then 80 characters."},
    {ERR_CODE_9, "Invalid arguments, the number of operands is incorrect."},
    {ERR_CODE_10, "Invalid arguments, the operand is not suitable for the operation."},
    {ERR_CODE_11, "Invalid arguments, the number is not supported."},
    {ERR_CODE_12, "Invalid arguments, the number is out of range."},
    {ERR_CODE_13, "Invalid registar name, the register dosent between $0-$31."},
    {ERR_CODE_14, "Out of range."},
    {ERR_CODE_15, "Out of memory."},
    {ERR_CODE_16, "Comma in the wrong place."},
    {ERR_CODE_17, "There is more commas then needed."},
    {ERR_CODE_18, "Missing '\"' after '.string'."},
    {ERR_CODE_19, "Missing '\"' at the end of '.string' line."},
    {ERR_CODE_20, "Invalid mcro name, identical to an lable name."},
    {ERR_CODE_21, "Instruction does not exist."},
    {ERR_CODE_22, "Invalid label name, does not start with a letter."},
    {ERR_CODE_23, "Invalid labal name, there is an unauthorized character."},
    {ERR_CODE_24, "Invalid label name, longer than 31 characters."},
    {ERR_CODE_25, "Invalid label name, identical to an instruction name."},
    {ERR_CODE_26, "Invalid label definition, there is ' ' between labal name and ':'."},
    {ERR_CODE_27, "Invalid label definition, misiing ':' after lable name."},
    {ERR_CODE_28, "Label definition repeats more then once."},
    {ERR_CODE_29, "A symbol not defined in the symbol table was used."},
    {ERR_CODE_30, "A symbol defined as an '.entry' was redefined as an '.extern'."},
    {ERR_CODE_31, "A symbol defined as an '.extern' was redefined as an '.entry'."},
    {ERR_CODE_LAST, "CHECK."}, 
};

void err_reset(void){
    err_count = 0;
}

void err_report(Line line, ERROR_CODES error_code){
    printf("Error [%s:%d]: %s\n", line.file_name, line.line_num, errors[error_code].err_msg);
    err_count++;
}

Bool err_found(void){
    return (err_count>0) ? TRUE : FALSE ;
}

void print_summary_err(void){
    if(err_count>0){
        printf("%d ERRORS WERE FOUND!\n", err_count);
    }
    else{
        printf("%s\n", errors[CODE_0].err_msg);
    }
}