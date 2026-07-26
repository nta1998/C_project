#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include "globals.h"

/**
 * All the error codes that could be found:
 * Each error code corresponds to a specific error message.
 * The error codes are used to identify and report errors encountered during the assembly process.
 * CODE_0 : The only code that reports a valid file.
 * ERR_CODE_1-ERR_CODE_7 : Errors during the PRE-ASSEMBLER stage.
 * ERR_CODE_8-ERR_CODE_28 : Errors during the FIRST-PASS stage.
 * ERR_CODE_29-ERR_CODE_31 : Errors during the SECOND-PASS stage.
 * ERR_CODE_LAST : Save for range check.
*/

typedef enum {
    CODE_0 = 0,
    ERR_CODE_1,
    ERR_CODE_2,
    ERR_CODE_3,
    ERR_CODE_4,
    ERR_CODE_5,
    ERR_CODE_6,
    ERR_CODE_7,
    ERR_CODE_8,
    ERR_CODE_9,
    ERR_CODE_10,
    ERR_CODE_11,
    ERR_CODE_12,
    ERR_CODE_13,
    ERR_CODE_14,
    ERR_CODE_15,
    ERR_CODE_16,
    ERR_CODE_17,
    ERR_CODE_18,
    ERR_CODE_19,
    ERR_CODE_20,
    ERR_CODE_21,
    ERR_CODE_22,
    ERR_CODE_23,
    ERR_CODE_24,
    ERR_CODE_25,
    ERR_CODE_26,
    ERR_CODE_27,
    ERR_CODE_28,
    ERR_CODE_29,
    ERR_CODE_30,
    ERR_CODE_31,
    ERR_CODE_LAST
} ERROR_CODES;

/**
 * An Error data structure.
 * @param err_code: an error code from the ERROR_CODES enum.
 * @param err_msg: a string with the error message corresponding to the error code.
*/

typedef struct {
    ERROR_CODES err_code;
    char *err_msg;
} Error;

/**
 * The function role is to reset the error counter.
 * The function is called whenever a new file is checked.
*/

void err_reset(void);

/**
 * The function role is to report an error found during the assembly process.
 * It prints the error message along with the file name and line number where the error occurred.
 * @param line: a Line structure containing the file name, line number, and line data.
 * @param error_code: an ERROR_CODES value representing the specific error encountered.
*/

void err_report(Line line, ERROR_CODES error_code);

/**
 * The function role is to check if any errors were found during the assembly process.
 * It returns TRUE if at least one error was encountered, and FALSE otherwise.
 * This function is used to determine whether to generate output files or skip output generation.
 * @return a Bool indicating whether errors were found (TRUE) or not (FALSE).
*/

Bool err_found(void);

/**
 * The function role is to print a summary of the errors found during the assembly process.
 * It provides an overview of the total number of errors encountered.
 * This function is called after processing the file to give feedback on the assembly process.
*/

void print_summary_err(void);

#endif