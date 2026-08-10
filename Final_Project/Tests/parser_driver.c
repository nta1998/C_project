/* parser_driver.c - דרייבר בדיקה מבודד ל-parser.
 *
 * מריץ את line_split לבד על כל שורה בקובץ, ומדפיס איך כל שורה פורקה:
 * סוג השורה, התווית, שם ההוראה והאופרנדים.
 *
 * זה השלב היחיד שבאמת מבודד: הוא לא נוגע בטבלת הסמלים ולא בתמונת
 * הזיכרון, ולכן כל מה שרואים כאן הוא תוצאה של הפרסר בלבד.
 *
 * שימושי לאיתור באגים שקשה לראות דרך התוכנית המלאה: כאן רואים ישירות
 * אם שדה נשאר משורה קודמת, אם תווית נקלטה בטעות כשם הוראה, או אם
 * מספר השורה שהפרסר מדווח אינו מספר השורה האמיתי.
 *
 * הקלט הוא קובץ .am (אחרי פריסת מאקרו). אפשר להזין גם .as רגיל,
 * כל עוד אין בו הגדרות mcro.
 *
 * בנייה והרצה:
 *     make parser_driver FILE=Tests/e2e/valid_basic.as
 *     ./build/parser_driver <file>
 */

#include <stdio.h>

#include "../Headers/globals.h"
#include "../Headers/parser.h"
#include "../Headers/errors.h"

/* שמות סוגי השורות, לפי הסדר ב-enum LineKind */
static const char *kind_name(int k)
{
    switch (k) {
        case LINE_INVALID:     return "INVALID";
        case LINE_EMPTY:       return "EMPTY";
        case LINE_COMMENT:     return "COMMENT";
        case LINE_DIRECTIVE:   return "DIRECTIVE";
        case LINE_INSTRUCTION: return "INSTRUCTION";
        default:               return "?";
    }
}

/* מדפיס שדה מחרוזת, או '-' אם הוא ריק. בלי זה קשה להבחין בין
 * שדה שנשאר ריק לבין שדה שהפרסר מילא ברווחים. */
static const char *or_dash(const char *s)
{
    return (s[0] == '\0') ? "-" : s;
}

int main(int argc, char *argv[])
{
    FILE *am;
    char line[MAX_LINE_LEN + 2];
    Parsed_line pl;
    Line e;
    int line_num = 0;
    int split_ok = 0, split_failed = 0;
    int i;

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

    printf("\n=== parser: line by line ===\n\n");
    printf("  %-4s %-12s %-14s %-10s %s\n",
           "line", "kind", "label", "name", "rest");
    printf("  ---- ------------ -------------- ---------- ----\n");

    while (fgets(line, sizeof(line), am) != NULL) {
        line_num++;

        e.file_name = argv[1];
        e.line_num  = line_num;
        e.data      = line;

        /* מאפסים את המבנה לפני כל שורה. שדה שנשאר משורה קודמת הוא
         * בדיוק סוג הבאג שהדרייבר הזה אמור לחשוף, ובלי איפוס לא
         * היה אפשר להבחין בינו לבין שדה שהפרסר מילא כראוי. */
        for (i = 0; i < (int)sizeof(pl); i++) {
            ((char *)&pl)[i] = 0;
        }

        if (!line_split(line, &pl, e)) {
            printf("  %-4d %s\n", line_num, "<line_split returned FALSE>");
            split_failed++;
            continue;
        }

        printf("  %-4d %-12s %-14s %-10s %s\n",
               line_num, kind_name(pl.kind),
               or_dash(pl.label), or_dash(pl.name), or_dash(pl.rest));
        split_ok++;
    }

    fclose(am);

    printf("\n%d line(s) split, %d rejected\n", split_ok, split_failed);

    printf("\n");
    print_summary_err();
    return err_found() ? 1 : 0;
}
