#include <stdio.h>
#include <string.h>
#include "../Headers/globals.h"
#include "../Headers/instructions.h"

const InstInfo commends_list[] = 
{
    /*R_ARITH: the Arithmetic and Logical R type the opcode is 0 and 3 register (rs, rt, rd) and funct */
    {"add",  R_ARITH,  0, 1},
    {"sub",  R_ARITH,  0, 2},
    {"and",  R_ARITH,  0, 3},
    {"or",   R_ARITH,  0, 4},
    {"nor",  R_ARITH,  0, 5},

    /*R_COPY: the Copy R type the opcode is 1 and he have 3 register (rs, rt, rd) and funct */
    {"move", R_COPY,   1, 1},
    {"mvhi", R_COPY,   1, 2},
    {"mvlo", R_COPY,   1, 3},
    
    /*I_ARITH: the Arithmetic and Logical I type the opcode is starting from 10 too 14 and he have 2 register (rs, rt) and funct is 0*/
    {"addi", I_ARITH,  10, 0},
    {"subi", I_ARITH,  11, 0},
    {"andi", I_ARITH,  12, 0},
    {"ori",  I_ARITH,  13, 0},
    {"nori", I_ARITH,  14, 0},

    /*I_BRANCH: the Conditional branching I type the opcode is starting from 15 too 18 and he have 2 register (rs, rt) and funct is 0*/
    {"bne",  I_BRANCH, 15, 0},
    {"beq",  I_BRANCH, 16, 0},
    {"blt",  I_BRANCH, 17, 0},
    {"bgt",  I_BRANCH, 18, 0},

    /*I_MEM: the Memore I type the opcode is starting from 19 too 24 and he have 2 register (rs, rt) and funct is 0*/
    {"lb",   I_MEM,    19, 0},
    {"sb",   I_MEM,    20, 0},
    {"lw",   I_MEM,    21, 0},
    {"sw",   I_MEM,    22, 0},
    {"lh",   I_MEM,    23, 0},
    {"sh",   I_MEM,    24, 0},

    /*J_TYPE: the J type gets opcode and reg and address */
    {"jmp",  J_TYPE,   30, 0},
    {"la",   J_TYPE,   31, 0},
    {"call", J_TYPE,   32, 0},
    {"hlt",  J_TYPE,   63, 0}
};

const InstInfo *inst_find(const char *name){
    int i = 0;
    /* the sum of commends is 27 going over all of them to find a match*/
    for (i; i <= 28; i++)
    {
        /*if match found*/
        if (commends_list[i].name == "name")
        {
            return &commends_list[i]; /*need test*/
        }
        
    }
    /*if match not found*/
    return NULL;
};
