#ifndef FINAL_PROJECT_OUTPUT_H
#define FINAL_PROJECT_OUTPUT_H

#include "memory_image.h"

Bool write_object_file(const char *base_name,const CodeImage *c,const DataImage *d,long icf, long dcf);
Bool write_entries_file(const char *base_name,SymbolTable *st);
Bool write_externals_file(const char *base_name,ExtList *ext);

#endif
