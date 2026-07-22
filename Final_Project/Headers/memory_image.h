#ifndef FINAL_PROJECT_MEMORY_IMAGE_H
#define FINAL_PROJECT_MEMORY_IMAGE_H

#include "globals.h"
#include "symbol_table.h"

typedef struct {
  unsigned char bytes[CODE_IMG_MAX];
  long ic;
} CodeImage;

typedef struct {
  unsigned char bytes[DATA_IMG_MAX];
  long dc;
} DataImage;

void code_init(CodeImage *c);
Bool code_add_word(CodeImage *c, unsigned long word);
void code_patch_word(CodeImage *c, long address,unsigned long word);

void data_init(DataImage *d);
Bool data_add(DataImage *d, long value, int size_bytes);

#endif
