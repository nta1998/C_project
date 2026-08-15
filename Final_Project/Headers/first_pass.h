/**
 * @file first_pass.h
 * @brief Machine-code word layouts and the first-pass entry point.
 *
 * Each union overlays a full 32-bit word ('all_in_one') on its bit-fields
 * ('Fields'), so a word can be built field by field and then emitted, or
 * read back, as a single value.
 */
#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "globals.h"

/**
 * An R-type instruction word.
 * Layout: opcode(6) rs(5) rt(5) rd(5) funct(5) unuse(6).
 */
union machine_code_R{

    unsigned long all_in_one;
    
    struct {
        unsigned int unuse: UNUSE_BITS;        /* 5-0   */
        unsigned int funct: FUNCT_BITS;       /* 10-6  */
        unsigned int rd: REG_FIELD_BITS;     /* 15-11 */
        unsigned int rt: REG_FIELD_BITS;    /* 20-16 */
        unsigned int rs: REG_FIELD_BITS;   /* 25-21 */
        unsigned int opcode: OPCODE_BITS; /* 31-26 */
    } Fields;
};

/**
 * An I-type instruction word.
 * Layout: opcode(6) rs(5) rt(5) immed(16).
 */
union machine_code_I {

    unsigned long all_in_one;

    struct {
        unsigned int immed: IMMEDIATE_BITS;     /* 16-0  */
        unsigned int rt: REG_FIELD_BITS;       /* 20-16 */
        unsigned int rs: REG_FIELD_BITS;      /* 25-21 */
        unsigned int opcode: OPCODE_BITS;    /* 31-26 */
    } Fields;
};
/**
 * A J-type instruction word.
 * Layout: opcode(6) reg(1) address(25).
 */
union machine_code_J {
    unsigned long all_in_one;

    struct {
        unsigned int address: JTYPE_ADDRESS_BITS; /*  24-0 */
        unsigned int reg: JTYPE_REG_BITS;        /*   25  */
        unsigned int opcode: OPCODE_BITS;       /* 31-26 */
    } Fields;
};

/**
 * Performs the first pass over the post-macro (.am) file: builds the symbol
 * table, encodes the instruction words and accumulates the data image.
 * Operand values that depend on a symbol are left as placeholders for the
 * second pass to fill in.
 * @param am: the open .am file to read.
 * @param file_name: the source file name, used in error messages.
 * @param icf_curr_line: out-param, receives the final instruction counter.
 * @param dcf_curr_line: out-param, receives the final data counter.
 * @return TRUE if no error was reported during the pass, FALSE otherwise.
*/
Bool first_pass(FILE *am, const char *file_name, int *icf_curr_line, int *dcf_curr_line);

#endif