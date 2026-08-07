#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "globals.h"

#define IMMEDIATE_FIELD_MASK   0xFFFFUL
#define ADDRESS_FIELD_MASK 0x1FFFFFFUL

Bool second_pass(const char *file_name);


#endif