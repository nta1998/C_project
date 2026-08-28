# רשימת ה-Elements — כל מה שצריך להעלות ל-Higgsfield

**הכלל היחיד שאסור לשבור:** השם ב-Higgsfield → Elements זהה **בדיוק** לשם ב-`@tag`
שבפרומפט, כולל אותיות גדולות וקטנות. אות אחת שונה = המודל לא מוצא את הרפרנס
והפנים מתחלפות.

**24 Elements** בסך הכל, לשש סצנות. אחרי כל אחד: מחיקת פנים מפאנלי הגוף המלא,
והדבקת הפנים האמיתיות על פאנל הקלוז-אפ.

---

## סצנה 1 · הנמל — 3 נכסים

| Element | מודל | הגדרות | קלט | פרומפט |
|---|---|---|---|---|
| `@He_cruise` | Seedream 5.0 Pro | 16:9 · 2K | תמונות אמיתיות שלו | [`he-cruise-sheet-short.prompt.txt`](he-cruise-sheet-short.prompt.txt) |
| `@She_cruise` | Seedream 5.0 Pro | 16:9 · 2K | תמונות אמיתיות שלה | [`01-stage1-asset-prompts.md`](01-stage1-asset-prompts.md) §1.2 |
| `@cruise` | Soul Cinema | 21:9 · 1080p | טקסט בלבד | [`01-stage1-asset-prompts.md`](01-stage1-asset-prompts.md) §1.3 |

**שני אלה הם היסוד של כל הסרט.** כל דמות בכל תקופה אחרת נגזרת מ-`@He_cruise`
ו-`@She_cruise` בתיוג. אם הם לא מדויקים — שום דבר אחר לא יהיה.

---

## סצנה 2 · הזירה — 5 נכסים

| Element | מודל | הגדרות | קלט | פרומפט |
|---|---|---|---|---|
| `@He_arena` | Seedream 5.0 Pro | 16:9 · 2K | תייג `@He_cruise` | [`he-arena-sheet-tagged.prompt.txt`](he-arena-sheet-tagged.prompt.txt) |
| `@She_arena` | Seedream 5.0 Pro | 16:9 · 2K | תייג `@She_cruise` | [`she-arena-sheet-tagged.prompt.txt`](she-arena-sheet-tagged.prompt.txt) |
| `@robot` | Seedream 5.0 Pro | 16:9 · 2K | טקסט בלבד | [`robot-sheet.prompt.txt`](robot-sheet.prompt.txt) |
| `@arena` | Soul Cinema | 21:9 · 1080p | טקסט בלבד | [`arena-plate.prompt.txt`](arena-plate.prompt.txt) |
| `@sketch` | **GPT Image 2** | 16:9 · high | תייג `@robot` + `@He_arena` | [`arena-sketch.prompt.txt`](arena-sketch.prompt.txt) |

**`@sketch` אחרון** — הוא מקבל את שני הדפים שכבר נוצרו כקלט.

---

## סצנה 3 · הרכבת — 4 נכסים

| Element | מודל | הגדרות | קלט | פרומפט |
|---|---|---|---|---|
| `@He_train` | Seedream 5.0 Pro | 16:9 · 2K | תייג `@He_cruise` | [`he-train-sheet-tagged.prompt.txt`](he-train-sheet-tagged.prompt.txt) |
| `@She_train` | Seedream 5.0 Pro | 16:9 · 2K | תייג `@She_cruise` | [`she-train-sheet-tagged.prompt.txt`](she-train-sheet-tagged.prompt.txt) |
| `@train` | Soul Cinema | 21:9 · 1080p | טקסט בלבד | [`train-plate.prompt.txt`](train-plate.prompt.txt) |
| `@boards` | **GPT Image 2** | 16:9 · high | תייג `@train` + `@He_train` + `@She_train` | [`train-boards.prompt.txt`](train-boards.prompt.txt) |

**`@boards` אחרון** — הוא מקבל את שלושת הקודמים.

---

## סצנה 5 · הקרנבל — 3 נכסים

| Element | מודל | הגדרות | קלט | פרומפט |
|---|---|---|---|---|
| `@He_carnival` | Seedream 5.0 Pro | 16:9 · 2K | תייג `@He_cruise` | [`he-carnival-sheet-tagged.prompt.txt`](he-carnival-sheet-tagged.prompt.txt) |
| `@She_carnival` | Seedream 5.0 Pro | 16:9 · 2K | תייג `@She_cruise` | [`she-carnival-sheet-tagged.prompt.txt`](she-carnival-sheet-tagged.prompt.txt) |
| `@carnival` | Soul Cinema | 16:9 · 1080p | טקסט בלבד | [`01-stage1-asset-prompts.md`](01-stage1-asset-prompts.md) §1.4 |

---

## סצנה 7 · המטבח — 4 נכסים

| Element | מודל | הגדרות | קלט | פרומפט |
|---|---|---|---|---|
| `@He_kitchen` | Seedream 5.0 Pro | 16:9 · 2K | תייג `@He_cruise` | [`he-kitchen-sheet-tagged.prompt.txt`](he-kitchen-sheet-tagged.prompt.txt) |
| `@She_kitchen` | Seedream 5.0 Pro | 16:9 · 2K | תייג `@She_cruise` | [`she-kitchen-sheet-tagged.prompt.txt`](she-kitchen-sheet-tagged.prompt.txt) |
| `@kitchen` | Soul Cinema | 21:9 · 1080p | טקסט בלבד | [`kitchen-plate.prompt.txt`](kitchen-plate.prompt.txt) |
| `@olives` | Seedream 5.0 Pro | 16:9 · 2K | טקסט בלבד | [`olives-plate.prompt.txt`](olives-plate.prompt.txt) |

---

## סצנה 8 · אחרי הקרדיטים — 5 נכסים

| Element | מודל | הגדרות | קלט | פרומפט |
|---|---|---|---|---|
| `@He_today` | Seedream 5.0 Pro | 16:9 · 2K | תייג `@He_cruise` | [`he-today-sheet-tagged.prompt.txt`](he-today-sheet-tagged.prompt.txt) |
| `@She_today` | Seedream 5.0 Pro | 16:9 · 2K | תייג `@She_cruise` | [`she-today-sheet-tagged.prompt.txt`](she-today-sheet-tagged.prompt.txt) |
| `@boys` | Seedream 5.0 Pro | 16:9 · 2K | תייג `@He_today` + `@She_today` | [`boys-sheet.prompt.txt`](boys-sheet.prompt.txt) |
| `@livingroom` | Soul Cinema | 21:9 · 1080p | טקסט בלבד | [`livingroom-plate.prompt.txt`](livingroom-plate.prompt.txt) |
| `@game` | **GPT Image 2** | 16:9 · high | טקסט בלבד | [`game-screen.prompt.txt`](game-screen.prompt.txt) |

**`@boys` שלישי** — הוא מקבל את שני ההורים.
**`@game` ב-GPT Image 2 ולא ב-Seedream** — הסצנה תלויה בטקסט קריא על המסך, ושם הטקסט אמין.

---

## נכסים שכבר לא בשימוש

סצנת המעלית ירדה מהסרט (היא התנגשה עם המטבח — שתיהן מפגש ראשון בימינו).
`@He_elevator`, `@She_elevator`, `@elevator`, `@headphones` — **אל תעלה אותם.**

---

## סדר עבודה מומלץ

1. **`@He_cruise` + `@She_cruise` קודם כל.** כל שאר הדמויות נגזרות מהם בתיוג.
2. אחרי כל דף דמות: **מחק פנים** מפאנלי הגוף המלא, **הדבק פנים אמיתיות** על הקלוז-אפ.
3. לוחות לוקיישן אפשר להריץ במקביל — הם לא תלויים בכלום.
4. הנכסים עם `GPT Image 2` תמיד אחרונים בסצנה שלהם.
5. אחרי כל סצנה: העלה את הנכסים שלה ל-Elements **בשמות המדויקים**, ואז הפרומפט.

## סיכום

| | נכסים | וידאו |
|---|---|---|
| 1 · הנמל | 3 | 18s |
| 2 · הזירה | 5 | 18s |
| 3 · הרכבת | 4 | 18s |
| 5 · הקרנבל | 3 | 18s |
| 7 · המטבח | 4 | 18s |
| 8 · אחרי הקרדיטים | 5 | 25s |
| **סה"כ** | **24** | **~115 שניות** |
