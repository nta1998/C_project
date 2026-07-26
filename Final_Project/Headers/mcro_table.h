#ifndef MCRO_TABLE_H
#define MCRO_TABLE_H

#include <stddef.h>
#include "globals.h"

/**
 * A Mcro data structure.
 * @param mcro_name: a string with the maximum allowed length for a mcro name.
 * @param data_lines: an array—sized according to the maximum number of mcro lines—containing strings of the maximum line length per one.
 * @param line_counter: a line counter for each individual macro.
 * @param next: a pointer to the next macro in the linked list.
*/

typedef struct Mcro {
    char mcro_name[MAX_MCRO_LEN+1];
    char data[MAX_MCRO_LINES][MAX_LINE_LEN];
    int line_counter;
    struct Mcro *next;
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
 * @return a pointer to the newly created Mcro record.
*/

Mcro *mcro_add(const char *name);

/**
 * The function is called repeatedly until the 'endmcro' is reached.
 * Its role is to add lines of text related to the current macro definition,
 * based on the pointer returned by the previous function call.
 * @param mcro: pointer to the mcro record to which this line should be added.
 * @param line: pointer to current line that related to the current macro.
 * @return a Bool.
*/

Bool mcro_add_line(Mcro *mcro, const char *line);

/**
 * Searches the macro table (linked list) for a macro by name.
 * @param name: the macro name to search for.
 * @return a pointer to the matching Mcro record, or NULL if not found.
*/
Mcro *mcro_search(const char *name);

#endif 
