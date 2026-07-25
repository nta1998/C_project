/**
 * @file instructions.c
 * @brief  This file contains the implementation of the instruction search function.
 * It defines a table of instruction information and provides a function to search for instructions by name.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "globals.h"
#include "instructions.h"

/**
 * A static array of INSTRUCTION_INFO structures representing the instruction table.
 * Each entry contains the instruction name, type, opcode, function code, and number of operands.
*/
static INSTRUCTION_INFO instruction_table[] = {
    {"add", R_A_TYPE, 0, 1, 3},
    {"sub", R_A_TYPE, 0, 2, 3},
    {"and", R_A_TYPE, 0, 3, 3},
    {"or", R_A_TYPE, 0, 4, 3},
    {"nor", R_A_TYPE, 0, 5, 3},

    {"move", R_C_TYPE, 1, 1, 2},
    {"mvhi", R_C_TYPE, 1, 2, 2},
    {"mvlo", R_C_TYPE, 1, 3, 2},

    {"addi", I_A_TYPE, 10, -1, 2},
    {"subi", I_A_TYPE, 11, -1, 2},
    {"andi", I_A_TYPE, 12, -1, 2},
    {"ori", I_A_TYPE, 13, -1, 2},
    {"nori", I_A_TYPE, 14, -1, 2},

    {"bne", I_B_TYPE, 15, -1, 2},
    {"beq", I_B_TYPE, 16, -1, 2},
    {"blt", I_B_TYPE, 17, -1, 2},
    {"bgt", I_B_TYPE, 18, -1, 2},

    {"lb", I_M_TYPE, 19, -1, 2},
    {"sb", I_M_TYPE, 20, -1, 2},
    {"lw", I_M_TYPE, 21, -1, 2},
    {"sw", I_M_TYPE, 22, -1, 2},
    {"lh", I_M_TYPE, 23, -1, 2},
    {"sh", I_M_TYPE, 24, -1, 2},

    {"jmp", J_TYPE, 30, -1, 1},
    {"la", J_TYPE, 31, -1, 1},
    {"call", J_TYPE, 32, -1, 1},
    {"hlt", J_TYPE, 63, -1, 0}
};

INSTRUCTION_INFO *instruction_search(const char *curr_name){
    int i;
    for(i=0; i<COMMANDES_COUNT; i++){
        if(strcmp(instruction_table[i].name, curr_name) == 0){
            return &instruction_table[i];
        }
    }
    return NULL;
}

