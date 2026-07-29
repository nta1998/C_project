#ifndef EXTERNAL_TABLE_H
#define EXTERNAL_TABLE_H

#include "globals.h"

/**
 * An External data structure.
 * @param symbol_name: a string with the maximum allowed length for a symbol name.
 * @param address: a long integer representing the address associated with the external symbol.
 * @param next: a pointer to the next external symbol in the linked list.
*/

typedef struct External {
    char symbol_name[MAX_LABEL_LEN+1];
    long address;
    struct External *next;
} External;

/**
 * The function role is to reset the external table.
 * The function is called whenever a new file is checked.
*/

void external_table_reset(void);

/**
 * The function is called whenever a new external symbol is detected.
 * It opens a new, empty record in the external table and returns a pointer to that record,
 * enabling the addition of external symbol-related information as needed.
 * @param name: pointer to current external symbol name.
 * @param address: long integer representing the address associated with the external symbol.
 * @return a pointer to the newly created External record.
*/

External *external_add(const char *name, long address);

/**
 * The function returns the head of the external table linked list.
 * @return a pointer to the head of the external table linked list.
*/

External *external_table_get_head(void);

#endif
