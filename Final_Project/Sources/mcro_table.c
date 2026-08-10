/**
 * @file mcro_table.c
 * @brief This file contains the implementation of the macro table functions.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../Headers/mcro_table.h"

/* Static variable (limited to this file only) – store the current macro table. */
static Mcro *head = NULL;

void mcro_table_reset(void){
    Mcro *curr = head;
    Mcro *next_node;
    while (curr != NULL){
        next_node = curr->next; 
        free(curr);
        curr = next_node;
    }
    head = NULL;
}

Mcro *mcro_add(const char *name){
    Mcro *new_mcro = malloc(sizeof(Mcro));
    if (new_mcro == NULL){
        return NULL;
    }
    strcpy(new_mcro->mcro_name, name);
    new_mcro->line_counter = 0;
    new_mcro->next = head;
    head = new_mcro;
    return new_mcro;
}

Bool mcro_add_line(Mcro *mcro, const char*line){
    if (mcro == NULL){
        return FALSE; 
    }
    if (mcro->line_counter >= MAX_MCRO_LINES){
        return FALSE;
    }
    strcpy(mcro->data[mcro->line_counter], line);
    mcro->line_counter++;
    return TRUE;
}

Mcro *mcro_search(const char *name){
    Mcro *curr = head;
    while (curr != NULL){
        if (strcmp(curr->mcro_name, name) == 0){
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}
