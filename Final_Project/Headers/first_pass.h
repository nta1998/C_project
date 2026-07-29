#ifndef FINAL_PROJECT_FIRST_PASS_H
#define FINAL_PROJECT_FIRST_PASS_H

#include "globals.h"
#include "memory_image.h"
#include "symbol_table.h"


Bool first_pass(FILE *am, const char *filename, Symbol *st, CODE_LINE *code, DATA_LINE *data, long *icf_out, long *dcf_out);

#endif