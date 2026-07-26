/**
 * @file external_table.c
 * @brief This file contains the implementation of the external table functions.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "external_table.h"

/* Static variable (limited to this file only) – store the current symbol table of all symbols that have the attribute: S_EXTERNAL. */
static External *head = NULL;

void external_table_reset(void){
    External *curr = head;
    External *next_node;
    while (curr != NULL){
        next_node = curr->next; 
        free(curr);
        curr = next_node;
    }
    head = NULL;
}

External *external_add(const char *name, long address){
    External *new_external = malloc(sizeof(External));
    if (new_external == NULL){
        return NULL;
    }
    strcpy(new_external->symbol_name, name);
    new_external->address = address;
    new_external->next = head;
    head = new_external;
    return new_external;
}

External *external_table_get_head(void){
    return head;
}