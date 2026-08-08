/* fp_driver.c - דרייבר בדיקה מבודד ל-first_pass.
 *
 * מריץ את first_pass לבד, בלי הפרה-אסמבלר ובלי המעבר השני, ומדפיס את
 * כל מה ש-first_pass ייצר: ערך ההחזרה, ICF/DCF, טבלת הסמלים המלאה,
 * תמונת הקוד ותמונת הנתונים.
 *
 * שימושי לאיתור באגים שקשה לראות דרך התוכנית המלאה: כאן רואים ישירות
 * אילו סמלים נכנסו לטבלה, באיזו כתובת, ועם איזו תכונה.
 *
 * הקלט הוא קובץ .am (אחרי פריסת מאקרו). אפשר להזין גם .as רגיל,
 * כל עוד אין בו הגדרות mcro.
 *
 * בנייה והרצה:
 *     make fp_driver FILE=Tests/e2e/valid_basic.as
 *     ./build/fp_driver <file>
 */

#include <stdio.h>

#include "../Headers/globals.h"
#include "../Headers/first_pass.h"
#include "../Headers/errors.h"
#include "../Headers/symbol_table.h"
#include "../Headers/memory_image.h"

/* שמות התכונות, לפי הסדר ב-enum Attribute */
static const char *attr_name(int a)
{
    switch (a) {
        case S_CODE:     return "CODE";
        case S_DATA:     return "DATA";
        case S_EXTERNAL: return "EXTERNAL";
        default:         return "?";
    }
}

int main(int argc, char *argv[])
{
    FILE *am;
    int icf = 0, dcf = 0;
    Bool ok;
    Symbol *s;
    const Code_line *code;
    const Data_line *data;
    int i, count;

    if (argc < 2) {
        printf("usage: %s <file.am>\n", argv[0]);
        return 1;
    }

    am = fopen(argv[1], "r");
    if (am == NULL) {
        printf("cannot open '%s'\n", argv[1]);
        return 1;
    }

    err_reset();
    symbol_table_reset();
    memory_image_reset();

    ok = first_pass(am, argv[1], &icf, &dcf);
    fclose(am);

    printf("\n=== first_pass returned %s ===\n", ok ? "TRUE" : "FALSE");
    printf("ICF = %d   DCF = %d\n", icf, dcf);

    printf("\n--- symbol table ---\n");
    s = symbol_table_get_head();
    if (s == NULL) {
        printf("  (empty - no symbols were added)\n");
    }
    while (s != NULL) {
        printf("  %-32s value=%-6ld attr=%-8s entry=%d\n",
               s->symbol_name, s->value, attr_name(s->attribute), s->is_entry);
        s = s->next;
    }

    count = get_IC_count();
    printf("\n--- code image (%d lines) ---\n", count);
    code = get_code_image();
    for (i = 0; i < count; i++) {
        printf("  %04d  %08lX   %s\n",
               code[i].address, code[i].machine_code, code[i].source_code);
    }

    count = get_DC_count();
    printf("\n--- data image (%d lines) ---\n", count);
    data = get_data_image();
    for (i = 0; i < count; i++) {
        printf("  %04d  %08lX   (%d byte%s)\n",
               data[i].address, data[i].machine_code,
               data[i].byte_size, data[i].byte_size == 1 ? "" : "s");
    }

    printf("\n");
    print_summary_err();
    return ok ? 0 : 1;
}
