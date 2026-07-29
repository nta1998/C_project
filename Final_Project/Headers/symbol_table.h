#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "globals.h"

/**
 * Possible attributes for a symbol - where it was defined.
 * @param S_CODE: the symbol was defined in the code segment.
 * @param S_DATA: the symbol was defined in the data segment.
 * @param S_EXTERNAL: the symbol was declared via a .extern directive.
*/

typedef enum {
    S_CODE,
    S_DATA,
    S_EXTERNAL
} Attribute;

/**
 * A Symbol data structure.
 * @param symbol_name: a string with the maximum allowed length for a symbol name.
 * @param value: a long integer representing the value associated with the symbol.
 * @param attribute: an enumerated type indicating the attribute of the symbol (code, data, or external).
 * @param is_entry: a boolean indicating whether the symbol is an entry point.
 * @param next: a pointer to the next symbol in the linked list.
*/

typedef struct Symbol {
    char symbol_name[MAX_LABEL_LEN+1];
    long value;
    Attribute attribute;
    Bool is_entry;
    struct Symbol *next;
} Symbol;

/**
 * The function role is to reset the symbol table.
 * The function is called whenever a new file is checked.
*/

void symbol_table_reset(void);

/**
 * The function is called whenever a new symbol is detected.
 * It opens a new, empty record in the symbol table and returns a pointer to that record,
 * enabling the addition of symbol-related information as needed.
 * @param name: pointer to current symbol name.
 * @param attribute: enumerated type indicating the attribute of the symbol (code, data, or external).
 * @param value: long integer representing the value associated with the symbol.
 * @return a pointer to the newly created Symbol record.
*/

Symbol *symbol_add(const char *name, Attribute attribute, long value);

/**
 * The function is called after the first pass of the assembler.
 * Its role is to shift the values of all data symbols by the final instruction counter (IC) value,
 * ensuring that data symbols are correctly positioned in memory after the code segment.
 * @param final_ic: long integer representing the final instruction counter value after the first pass.
*/

void symbol_table_shift_data(long final_ic);

/**
 * Searches the symbol table (linked list) for a symbol by name.
 * @param name: the symbol name to search for.
 * @return a pointer to the matching Symbol record, or NULL if not found.
*/

Symbol *symbol_search(const char *name);

/**
 * Retrieves the head of the symbol table (linked list).
 * @return a pointer to the head of the Symbol linked list.
*/

Symbol *symbol_table_get_head(void);

#endif /* SYMBOL_TABLE_H */
