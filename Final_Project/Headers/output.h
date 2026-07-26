#ifndef OUTPUT_H
#define OUTPUT_H

#include "globals.h"

/**
 * Writes all output files (.ob, .ent, .ext) after a successful assembly
 * (both passes completed with no errors). The .ob file is always written;
 * .ent/.ext are written only if there is at least one relevant entry,
 * and removed again if they end up empty.
 * @param ob_file: the full path for the object (.ob) file.
 * @param ent_file: the full path for the entries (.ent) file.
 * @param ext_file: the full path for the externals (.ext) file.
*/

void write_output_files(const char *ob_file, const char *ent_file, const char *ext_file);

#endif
