#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "globals.h"
#include "symbol_table.h"
#include "memory_image.h"

static CODE_LINE code_image[MAX_LINES];
static int ic = START_ADDRESS;

static DATA_LINE data_image[MAX_LINES];
static int dc = 0;
