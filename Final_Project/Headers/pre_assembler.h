#ifndef FINAL_PROJECT_PRE_ASSEMBLER_H
#define FINAL_PROJECT_PRE_ASSEMBLER_H

#include "globals.h"

/* מריצה את שלב הקדם-הרכבה על source_path, וכותבת את הקובץ הפרוש
 * ל-output_path. מחזירה FALSE אם נמצאה שגיאה (למשל שם מאקרו לא חוקי) -
 * במקרה כזה אין ליצור עבור קובץ זה קבצי פלט נוספים (.ob/.ent/.ext). */
Bool preassemble(const char *as_path, const char *am_path);

#endif
