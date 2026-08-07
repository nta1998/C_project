#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include <stdio.h>
#include "globals.h"
#include "mcro_table.h"
#include "errors.h"

/**
 * The function role is to perform the pre-assembly stage of the assembler.
 * It reads the input assembly file, processes macro definitions and calls,
 * and generates an output file with expanded macros.
 * @param as_file: input assembly file name.
 * @param am_file: output file name after macro expansion.
 * @return a Bool indicating success (TRUE) or failure (FALSE).
*/
Bool pre_assembler(const char *as_file, const char *am_file);

#endif

