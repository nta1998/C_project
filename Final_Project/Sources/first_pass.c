#include <stdio.h>
#include <string.h>
#include "../Headers/globals.h"
#include "../Headers/first_pass.h"
#include "../Headers/parser.h"
#include "../Headers/errors.h"


Bool *is_valid_lable_def(Line curr_line){

    char lable_name[MAX_NAME_LEN+1];
    sscanf(curr_line.data, "%s", lable_name);

    if (!start_with_letter(&lable_name)){
        err_report(curr_line, ERR_CODE_22);
        return FALSE;
    } 
    if (!all_chars_valid_l(&lable_name)){
        err_report(curr_line, ERR_CODE_23);
        return FALSE;
    } 
    if (!is_len_valid(&lable_name)){
        err_report(curr_line, ERR_CODE_24);
        return FALSE;
    }
    if (is_reserved_word(&lable_name)){
        err_report(curr_line, ERR_CODE_25);
        return FALSE;
    } 
    if (strcmp(lable_name[-1], ":") != 0) {
        err_report(curr_line, ERR_CODE_26);
        return FALSE;
    }
    if (strcmp(lable_name[-2], " ") != 0) {
        err_report(curr_line, ERR_CODE_27);
        return FALSE;
    }
    if (symbol_search(lable_name) != NULL) {
        err_report(curr_line, ERR_CODE_28);
        return FALSE;
    }
    return TRUE;
}
