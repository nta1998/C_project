/**
 * @file symbol_table.c
 * @brief This file contains the implementation of the symbol table functions.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "symbol_table.h"

/* Static variable (limited to this file only) – store the current symbol table. */
static Symbol *head = NULL;

void symbol_table_reset(void){
    Symbol *curr = head;
    Symbol *next_node;
    while (curr != NULL){
        next_node = curr->next; 
        free(curr);
        curr = next_node;
    }
    head = NULL;
}

Symbol *symbol_add(const char *name, Attribute attribute, long value){
    Symbol *new_symbol = malloc(sizeof(Symbol));
    if (new_symbol == NULL){
        return NULL;
    }
    strcpy(new_symbol->symbol_name, name);
    new_symbol->value = value;
    new_symbol->attribute = attribute;
    new_symbol->is_entry = FALSE;
    new_symbol->next = head;
    head = new_symbol;
    return new_symbol;
}

void symbol_table_shift_data(long final_ic){
    Symbol *curr = head;
    while (curr != NULL){
        if (curr->attribute == S_DATA){
            curr->value += final_ic;
        }
        curr = curr->next;
    }
}

Symbol *symbol_search(const char *name){
    Symbol *curr = head;
    while (curr != NULL){
        if (strcmp(curr->symbol_name, name) == 0){
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

Symbol *symbol_table_get_head(void){
    return head;
}