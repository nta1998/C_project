#ifndef FINAL_PROJECT_FIRST_PASS_H
#define FINAL_PROJECT_FIRST_PASS_H

#include "globals.h"
#include "memory_image.h"

Bool first_pass(FILE *am, const char *filename, SymbolTable *st, CodeImage *code, DataImage *data, long *icf_out, long *dcf_out);
