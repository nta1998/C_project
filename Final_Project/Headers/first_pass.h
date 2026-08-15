#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "globals.h"

union machine_code_R{

    unsigned long all_in_one;
    
    struct {
        unsigned int unuse: UNUSE_BITS;       /* 5-0   */
        unsigned int funct: FUNCT_BITS;      /* 10-6  */
        unsigned int rd: REG_FIELD_BITS;        /* 15-11 */
        unsigned int rt: REG_FIELD_BITS;       /* 20-16 */
        unsigned int rs: REG_FIELD_BITS;      /* 25-21 */
        unsigned int opcode: OPCODE_BITS; /* 31-26 */
    } Fields;
};

union machine_code_I {

    unsigned long all_in_one;

    struct {
        unsigned int immed: IMMEDIATE_BITS;    /* 16-0 */
        unsigned int rt: REG_FIELD_BITS;       /* 20-16 */
        unsigned int rs: REG_FIELD_BITS;      /* 25-21 */
        unsigned int opcode: OPCODE_BITS; /* 31-26 */
    } Fields;
};
union machine_code_J {
    unsigned long all_in_one;

    struct {
        unsigned int address: JTYPE_ADDRESS_BITS; /* 24-0 */
        unsigned int reg: JTYPE_REG_BITS;     /* 25 */
        unsigned int opcode: OPCODE_BITS; /* 31-26 */
    } Fields;
};

Bool first_pass(FILE *am, const char *file_name, int *icf_curr_line, int *dcf_curr_line);

#endif