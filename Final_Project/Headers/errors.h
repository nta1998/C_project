#ifndef FINAL_PROJECT_ERRORS_H
#define FINAL_PROJECT_ERRORS_H

#include "globals.h"

/* Updates the flag to be 'false' at the beginning of a new file. */
void err_reset(void);

/* Report all errors appearing in a single file */
void err_report(const char *filename, int line_num, const char *format, ...);

/* Checks whether there were errors in the file */
Bool err_found(void);
