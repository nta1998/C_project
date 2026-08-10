#ifndef FINAL_PROJECT_FIRST_PASS_H
#define FINAL_PROJECT_FIRST_PASS_H
#include <stdio.h>
#include "globals.h"
#include "memory_image.h"
#include "symbol_table.h"
#include "parser.h"


union machine_code_R{

    unsigned long all_in_one;
    
    struct {
        unsigned int unuse: 6;       /* 5-0   */
        unsigned int funct: 5;      /* 10-6  */
        unsigned int rd: 5;        /* 15-11 */
        unsigned int rt: 5;       /* 20-16 */
        unsigned int rs: 5;      /* 25-21 */
        unsigned int opcode: 6; /* 31-26 */
    } fields;
};

union machine_code_I {

    unsigned long all_in_one;

    struct {
        unsigned int immed: 16;    /* 16-0 */
        unsigned int rt: 5;       /* 20-16 */
        unsigned int rs: 5;      /* 25-21 */
        unsigned int opcode: 6; /* 31-26 */
    } fields;
};
union machine_code_J {
    unsigned long all_in_one;

    struct {
        unsigned int address: 25; /* 24-0 */
        unsigned int reg: 1;     /* 25 */
        unsigned int opcode: 6; /* 31-26 */
    } fields;
};

Bool first_pass(FILE *am, const char *filename, int *icf_out, int *dcf_out);

void add_directive_data(int pram_num , char ops[][81],char name[],int *dc);

unsigned long to_binery(Parsed_line pl, char ops[][81], Line line);

#endif