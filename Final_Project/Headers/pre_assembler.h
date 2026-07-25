#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include <stdio.h>
#include "globals.h"
#include "mcro_table.h"
#include "errors.h"


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
 * The function called only if the flag `curr_mcro` == `NULL`,
 * and its role is to return the type of row involved : MCRO_START, MCRO_CALL or MCRO_END.
 * @param line: pointer to the current line.
 * @return an MCRO_OFF indicating the type of the line (MCRO_START, MCRO_CALL, or REGULAR).
*/
MCRO_OFF mcro_off(const char *line);

/**
 * The function called only if the flag `curr_mcro` != `NULL`,
 * and its role is to return the type of row involved : MCRO_BODY or MCRO_END.
 * @param line: pointer to the current line.
 * @return an MCRO_ON indicating the type of the line (MCRO_BODY or MCRO_END).
*/
MCRO_ON mcro_on(const char *line);

/**
 * The function checks whether the current line is a valid macro definition line.
 * @param curr_line_data: pointer to the current line data.
 * @return a Bool indicating whether the line is valid (TRUE) or not (FALSE).
*/
Bool valid_mcro_def_line(const char *curr_line_data);

/**
 * The function checks whether the current line is a valid macroend definition line.
 * @param curr_line_data: pointer to the current line data.
 * @return a Bool indicating whether the line is valid (TRUE) or not (FALSE).
*/
Bool valid_mcroend_def_line(const char *curr_line_data);

/**
 * The function expands a macro call by writing the corresponding macro lines to the output file.
 * @param line: pointer to the current line containing the macro call.
 * @param curr_am_file: pointer to the output file where the expanded macro lines will be written.
*/
void expand_mcro_call(const char *line, FILE *curr_am_file);

/**
 * The function checks whether the current line is a valid macro definition line.
 * @param curr_line: the current line's context (passed by value).
 * @return Mcro structure if the line is valid, or NULL if it is not.
*/
Mcro *start_mcro_def(Line curr_line);

/**
 * The function role is to perform the pre-assembly stage of the assembler.
 * It reads the input assembly file, processes macro definitions and calls,
 * and generates an output file with expanded macros.
 * @param as_file: input assembly file name.
 * @param am_file: output file name after macro expansion.
 * @return a Bool indicating success (TRUE) or failure (FALSE).
*/
Bool pre_assembler(const char *as_file, const char *am_file);

#endif

