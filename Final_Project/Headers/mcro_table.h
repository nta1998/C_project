#ifndef MCRO_TABLE_H
#define MCRO_TABLE_H

#include <stddef.h>
#include "globals.h"

/**
 * An Mcro data structure.
 * @param mcro_name: a string with the maximum allowed length for a mcro name.
 * @param data_lines: an array—sized according to the maximum number of mcro lines—containing strings of the maximum line length per one.
 * @param line_counter: a line counter for each individual macro.
*/

typedef struct {
    char mcro_name[MAX_MCRO_LEN+1];
    char data_lines[MAX_MCRO_LINES][MAX_LINE_LEN];
    int line_counter;
} Mcro;

/**
 * The function role is to reset the mcro table.
 * The function is called whenever a new file is checked.
*/

void mcro_table_reset(void);

/**
 * The function is called whenever a new mcro line is detected.
 * It opens a new, empty record in the mcro table and returns a pointer to that record,
 * enabling the addition of mcro-related lines of code as needed.
 * @param name: pointer to current mcro name.
 * @return a Mcro.
*/

Mcro *mcro_add(const char *name);

/**
 * The function is called repeatedly until the 'endmcro' is reached.
 * Its role is to add lines of text related to the current macro definition,
 * based on the pointer returned by the previous function call.
 * @param ncro: pointer to current mcro name.
 * @param line: pointer to current line that related to the current macro.
 * @return a Bool.
*/

Bool mcro_add_line(Mcro *mcro, const char *line);

/**
 * The function checks whether a mcro has been defined in the mcro table.
 * It returns 'NULL' if it has not been defined,
 * and if it has, it returns a pointer to the requested mcro.
 * @param mcro_table: pointer to the mcro table.
 * @param table_size: stores the table size.
 * @param mcro_name: pointer to current mcro.
 * @return a Mcro.
*/

Mcro *mcro_search(const char *name);

#endif 
