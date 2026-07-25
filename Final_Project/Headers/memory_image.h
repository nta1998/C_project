#ifndef MEMORY_IMAGE_H
#define MEMORY_IMAGE_H

#include "globals.h"
#include "symbol_table.h"

typedef struct {
    int address;
    char source_code[MAX_LINE_LEN+1];
    unsigned long machine_code;
} CODE_LINE;

typedef struct {
    int address;
    unsigned long machine_code;
} DATA_LINE;

#endif

void code_init(CODE_LINE *c);
Bool code_add_word(CODE_LINE *c, unsigned long word);
void code_patch_word(CODE_LINE *c, long address,unsigned long word);

void data_init(DATA_LINE *d);
Bool data_add(DATA_LINE *d, long value, int size_bytes);

