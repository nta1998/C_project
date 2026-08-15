#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "globals.h"

/* Bit-field masks used to patch a previously-encoded machine word with a resolved symbol value */
#define IMMEDIATE_FIELD_MASK   0xFFFFUL
#define ADDRESS_FIELD_MASK 0x1FFFFFFUL

/**
 * The function role is to perform the second pass stage of the assembler:
 * It re-reads the source file, resolves every label reference left unresolved
 * during the first pass (branch labels, jump/call labels and .entry directives) by patching the code image and registering external symbol usages,
 * and reports an error for any label that is undefined or used illegally (e.g. an external label used in a branch, or a missing .entry label).
 * @param file_name: the full path of the source (.am) file to re-read.
 * @return a Bool indicating whether the second pass completed with no errors (TRUE) or not (FALSE).
*/
Bool second_pass(const char *file_name);

#endif
