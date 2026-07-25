/**
 * @file mcro_table.c
 * @brief This file contains the implementation of the macro table functions.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "globals.h"
#include "mcro_table.h"

/* Static variable (limited to this file only) – store the current macro table. */
static Mcro table[MAX_MCROS];
static int mcro_count= 0;

void mcro_table_reset(void){
    mcro_count= 0;
}

Mcro *mcro_add(const char *name){
    if (mcro_count >= MAX_MCROS){
        return NULL;
    }
    strcpy(table[mcro_count].mcro_name, name);
    table[mcro_count].line_counter= 0;
    mcro_count++;
    return &table[mcro_count-1];
}

Bool mcro_add_line(Mcro *mcro, const char*line){
    if (mcro->line_counter >= MAX_MCRO_LINES){
        return FALSE;
    }
    strcpy(mcro->data_lines[mcro->line_counter], line);
    mcro->line_counter++;
    return TRUE;
}

Mcro *mcro_search(const char *name){
    int i;
    for(i=0; i<mcro_count; i++){
        if(strcmp(table[i].mcro_name, name) == 0){
            return &table[i];
        }
    }
    return NULL;
}