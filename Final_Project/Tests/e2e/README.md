# בדיקות קצה-לקצה (end-to-end)

בדיקות שמריצות את `assembler` על קבצי `.as` שלמים ומשוות את קבצי הפלט
(`.am` / `.ob` / `.ent` / `.ext`) לקבצים צפויים. מכסות את כל השרשרת:

    pre_assembler → first_pass → second_pass → output

## הרצה

```bash
bash Tests/e2e/run_e2e.sh              # כל הבדיקות
bash Tests/e2e/run_e2e.sh valid_data   # בדיקה אחת
bash Tests/e2e/run_e2e.sh --bless      # רישום הפלט הנוכחי כצפוי
```

הסקריפט לא נוגע בקבצי מקור ולא כותב לתוך `Tests/` — כל ההרצה מתבצעת
בתיקייה זמנית. קוד יציאה 0 = הכל עבר.

## מקרי הבדיקה

| קובץ | מה נבדק |
|---|---|
| `valid_basic.as` | הוראות מכל 6 הסוגים (R_A, R_C, I_A, I_B, I_M, J), תוויות, `.entry` |
| `valid_data.as` | `.db` / `.dh` / `.dw` / `.asciz`, ערכי קצה, יישור כתובות |
| `valid_macro.as` | הגדרת מאקרו, פריסה חוזרת, מאקרו רב-שורות |
| `valid_extern.as` | `.extern` ויצירת `.ext` עם כל מקומות השימוש |
| `valid_boundaries.as` | ערכי קצה: `$0`/`$31`, immediate בגבולות 16 ביט, `.db`/`.dh`/`.dw` במינימום ובמקסימום, אריזת בתים לא מיושרת |
| `valid_mixed_all.as` | מאקרו מעורב עם תווית באותה שורה, קפיצות קדימה ואחורה, נתונים אחרי הקוד |
| `error_macro.as` | שגיאות פרה-אסמבלר (קודים 1–8) |
| `error_syntax.as` | שגיאות מעבר ראשון (קודים 10–29) |
| `error_symbols.as` | שגיאות מעבר שני (קודים 30–33) |
| `error_operands.as` | שגיאות אופרנדים: רגיסטרים לא חוקיים, מספר אופרנדים, פסיקים, ערכים מחוץ לטווח |

מקרה ששמו מתחיל ב-`error_` נבדק אחרת: הוא **חייב** לדווח שגיאות, ו**אסור**
שייווצרו עבורו קבצי `.ob`/`.ent`/`.ext`. רשימת השגיאות עצמה מושווית גם היא,
כדי שרגרסיה תתגלה.

## תיקיית `expected/`

עדיין לא נוצרה. אי אפשר לרשום פלט צפוי לפני שהתוכנה רצה נכון (ראה למטה).
אחרי שהבאגים יתוקנו:

```bash
bash Tests/e2e/run_e2e.sh --bless
```

**חשוב:** `--bless` רק מצלם את הפלט הנוכחי. צריך לעבור על הקבצים ידנית
ולוודא שהם באמת נכונים לפני שסומכים עליהם — אחרת מקבעים באג כ"התנהגות צפויה".

## מה חוסם את ההרצה כרגע

הבדיקות כתובות ומוכנות, אבל התוכנה עדיין לא מגיעה לסוף. ארבעה חסמים:

**חתימת `first_pass` לא תואמת** — `assembler.c:97` קורא
`first_pass(as_file, am_file)` בשני ארגומנטים, אבל `first_pass.h`
מגדיר חמישה: `(FILE *am, const char *filename, Symbol *st, int *icf_out,
int *dcf_out)`. הארגומנט הראשון הוא `FILE *` פתוח, לא שם קובץ.

התיקון הצפוי:

```c
am_fp = fopen(am_file, "r");
if (am_fp == NULL){ continue; }
first_pass_ok = first_pass(am_fp, as_file, NULL, &icf, &dcf);
fclose(am_fp);
if (first_pass_ok && second_pass(am_file)){ ... }
```

כדאי לשים לב ש-`second_pass` צריך כנראה את `am_file` (אחרי פריסת המאקרו)
ולא את `as_file`.

## מה כבר תוקן

- `main` כפול — `main.c` ו-`fp_main.c` נמחקו; `assembler.c` הוא ה-main היחיד
- `is_reserved_word` כפול — נפתר
- `&mcro_name` ב-`pre_assembler.c` — נפתר
- `#include` חסר ב-`assembler.c` — נוסף, מה שהפך קריסת runtime לשגיאת קומפילציה
