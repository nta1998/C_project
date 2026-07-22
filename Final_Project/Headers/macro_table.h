#ifndef FINAL_PROJECT_MACRO_TABLE_H
#define FINAL_PROJECT_MACRO_TABLE_H

#include <stddef.h>
#include "globals.h"

typedef struct Macro {

} Macro;

typedef struct MacroTable { 

} MacroTable;

typedef struct MacroLine {

} MacroLine;

MacroTable *mtab_create(void);

/* יוצרת רשומת מאקרו חדשה וריקה. מחזירה NULL אם כבר קיים מאקרו בשם הזה. */
Macro *mtab_add(MacroTable *table, const char *name);

/* מחפשת מאקרו לפי שם. מחזירה NULL אם לא נמצא. */
Macro *mtab_find(MacroTable *table, const char *name);

Bool macro_append_line(Macro *macro, const char *line);
MacroLine *macro_first_line(Macro *macro);
MacroLine *line_next(MacroLine *line);
const char *line_text(MacroLine *line);
void mtab_free(MacroTable *table);

#endif 
