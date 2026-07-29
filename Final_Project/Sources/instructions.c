#include <stdio.h>
#include <string.h>
#include "../Headers/globals.h"
#include "../Headers/instructions.h"

const Instruction_info commands_list[] = 
{
    /*R_ARITH: the Arithmetic and Logical R type the opcode is 0 and 3 register (rs, rt, rd) and funct */
    {"add",  R_A_TYPE,  0, 1},
    {"sub",  R_A_TYPE,  0, 2},
    {"and",  R_A_TYPE,  0, 3},
    {"or",   R_A_TYPE,  0, 4},
    {"nor",  R_A_TYPE,  0, 5},

    /*R_COPY: the Copy R type the opcode is 1 and he have 3 register (rs, rt, rd) and funct */
    {"move", R_C_TYPE,   1, 1},
    {"mvhi", R_C_TYPE,   1, 2},
    {"mvlo", R_C_TYPE,   1, 3},
    
    /*I_ARITH: the Arithmetic and Logical I type the opcode is starting from 10 too 14 and he have 2 register (rs, rt) and funct is 0*/
    {"addi", I_A_TYPE,  10, 0},
    {"subi", I_A_TYPE,  11, 0},
    {"andi", I_A_TYPE,  12, 0},
    {"ori",  I_A_TYPE,  13, 0},
    {"nori", I_A_TYPE,  14, 0},

    /*I_BRANCH: the Conditional branching I type the opcode is starting from 15 too 18 and he have 2 register (rs, rt) and funct is 0*/
    {"bne",  I_B_TYPE, 15, 0},
    {"beq",  I_B_TYPE, 16, 0},
    {"blt",  I_B_TYPE, 17, 0},
    {"bgt",  I_B_TYPE, 18, 0},

    /*I_MEM: the Memore I type the opcode is starting from 19 too 24 and he have 2 register (rs, rt) and funct is 0*/
    {"lb",   I_M_TYPE,    19, 0},
    {"sb",   I_M_TYPE,    20, 0},
    {"lw",   I_M_TYPE,    21, 0},
    {"sw",   I_M_TYPE,    22, 0},
    {"lh",   I_M_TYPE,    23, 0},
    {"sh",   I_M_TYPE,    24, 0},

    /*J_TYPE: the J type gets opcode and reg and address */
    {"jmp",  J_TYPE,   30, 0},
    {"la",   J_TYPE,   31, 0},
    {"call", J_TYPE,   32, 0},
    {"hlt",  J_TYPE,   63, 0}
};
#define COMMANDS_COUNT (sizeof(commands_list) / sizeof(commands_list[0]))
const InstInfo *inst_find(const char *name)
{
    int i;

    if (curr_name == NULL)
    {
        return NULL;
    }

    for (i = 0; i < (int)COMMANDS_COUNT; i++)
    {
        if (strcmp(commands_list[i].name, curr_name) == 0)
        {
            return &commands_list[i];
        }
    }

    return NULL;
}