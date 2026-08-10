/* sp_driver.c - דרייבר בדיקה ל-second_pass.
 *
 * מריץ את second_pass בלי הפרה-אסמבלר, ומדפיס את טבלת הסמלים ותמונת
 * הקוד פעמיים: אחרי המעבר הראשון ואחרי המעבר השני. ההשוואה בין שתי
 * ההדפסות מראה בדיוק מה המעבר השני שינה - אילו כתובות הושלמו ואילו
 * סמלים סומנו כ-entry או כ-external.
 *
 * שים לב: בניגוד ל-fp_driver ול-parser_driver, השלב הזה אינו מבודד
 * ואינו יכול להיות. second_pass אינו מקבל את טבלת הסמלים כפרמטר -
 * הוא קורא את הטבלה ואת תמונת הקוד ש-first_pass בנה במשתנים גלובליים,
 * ולכן first_pass חייב לרוץ לפניו. אם המעבר הראשון נכשל, המעבר השני
 * יעבוד על טבלאות פגומות, ולכן במקרה כזה הדרייבר עוצר ואינו מריץ אותו.
 *
 * הקלט הוא קובץ .am (אחרי פריסת מאקרו). אפשר להזין גם .as רגיל,
 * כל עוד אין בו הגדרות mcro.
 *
 * בנייה והרצה:
 *     make sp_driver FILE=Tests/e2e/valid_basic.as
 *     ./build/sp_driver <file>
 */

#include <stdio.h>

#include "../Headers/globals.h"
#include "../Headers/first_pass.h"
#include "../Headers/second_pass.h"
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

/* מדפיס את טבלת הסמלים. נקרא פעמיים - לפני ואחרי המעבר השני -
 * כדי שההשוואה תהיה שורה מול שורה. */
static void print_symbols(void)
{
    Symbol *s = symbol_table_get_head();

    printf("\n--- symbol table ---\n");
    if (s == NULL) {
        printf("  (empty - no symbols were added)\n");
        return;
    }
    while (s != NULL) {
        printf("  %-32s value=%-6ld attr=%-8s entry=%d\n",
               s->symbol_name, s->value, attr_name(s->attribute), s->is_entry);
        s = s->next;
    }
}

/* מדפיס את תמונת הקוד. המעבר השני משנה את המילים שבהן היה סמל
 * שטרם נפתר, ולכן זו ההדפסה שבה רואים את עבודתו. */
static void print_code_image(void)
{
    const Code_line *code;
    int i, count;

    count = get_IC_count();
    printf("\n--- code image (%d lines) ---\n", count);
    code = get_code_image();
    for (i = 0; i < count; i++) {
        printf("  %04d  %08lX   %s\n",
               code[i].address, code[i].machine_code, code[i].source_code);
    }
}

/* מדפיס את תמונת הנתונים. */
static void print_data_image(void)
{
    const Data_line *data;
    int i, count;

    count = get_DC_count();
    printf("\n--- data image (%d lines) ---\n", count);
    data = get_data_image();
    for (i = 0; i < count; i++) {
        printf("  %04d  %08lX   (%d byte%s)\n",
               data[i].address, data[i].machine_code,
               data[i].byte_size, data[i].byte_size == 1 ? "" : "s");
    }
}

int main(int argc, char *argv[])
{
    FILE *am;
    int icf = 0, dcf = 0;
    Bool ok;

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

    /* --- המעבר הראשון: מכין את הטבלאות שהמעבר השני קורא --- */
    printf("\n=== first_pass (required - builds the tables) ===\n");
    ok = first_pass(am, argv[1], &icf, &dcf);
    fclose(am);

    printf("first_pass returned %s   ICF = %d   DCF = %d\n",
           ok ? "TRUE" : "FALSE", icf, dcf);

    /* אם המעבר הראשון נכשל, כל פלט של המעבר השני יטעה: הוא יעבוד על
     * טבלת סמלים חלקית. עדיף לעצור ולומר זאת מאשר להדפיס זבל. */
    if (!ok) {
        printf("\nfirst_pass FAILED - second_pass was not run.\n");
        printf("Fix the errors below first, then run this driver again.\n\n");
        print_summary_err();
        return 1;
    }

    printf("\n=== state after first_pass ===\n");
    print_symbols();
    print_code_image();
    print_data_image();

    /* --- המעבר השני --- */
    ok = second_pass(argv[1]);

    printf("\n=== state after second_pass (returned %s) ===\n",
           ok ? "TRUE" : "FALSE");
    print_symbols();
    print_code_image();

    printf("\n");
    print_summary_err();
    return ok ? 0 : 1;
}
