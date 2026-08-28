# שלב 2 — Elements ומסגרת הפרומפטים

## הזרימה (מהמדריך, מילה במילה)

> "Download the skill, then in Claude go to **Customize → Skills** and upload it.
> Open a fresh chat, attach your script and every asset you built in Stage 1, and give
> Claude the element list — **the names Claude uses in the prompt must match the names of
> your assets.** Then in Higgsfield add each asset under **Elements** with exactly the same
> names, and drop the finished prompt into **Cinema Studio**."

### אצלנו זה קצת יותר קל
הסקיל `higgsfield-seedance-prompt-builder` **כבר מותקן בסשן הזה**.
אין צורך להוריד ולהעלות כלום — פשוט תגיד לי איזו סצנה, תיתן לי את רשימת ה-Elements,
ותתאר את הסצנה כמו שהיית מספר אותה לחבר. אני מחזיר פרומפט Seedance שוט-אחר-שוט מוכן.

---

## רשימת ה-Elements

הכלל היחיד שאסור לשבור: **השם ב-Higgsfield → Elements זהה בדיוק לשם ב-`@tag` בפרומפט.**
אות אחת שונה = המודל לא ימצא את הרפרנס והפנים יתחלפו.

מבנה השמות במדריך המקורי (החלף `Nate`/`Aila` בשמות שלכם):

```
@Nate_cruise    + @Aila_cruise    + @cruise                                  — הנמל
@Nate_arena     + @Aila_arena     + @giant + @arena + @throne + @sketch      — הזירה
@Nate_train     + @Aila_train     + @train + @storyboard1..3                 — הרכבת
@Nate_ship      + @Aila_ship      + @duelist_ship + @ship + @deck
                                  + @spyglass + @door + @crew                — הפיראטים
@Nate_carnival  + @Aila_carnival  + @carnival                                — הקרנבל
@Nate_elevator  + @Aila_elevator  + @elevator + @headphones + @Audio         — המעלית
```

שים לב לתבנית:
- **דמות לכל תקופה** — `@{שם}_{תקופה}`. לא דמות אחת שמשמשת בכולן.
- **לוקיישן לכל תקופה** — שם קצר בלי קידומת.
- **פרופים שחוזרים** מקבלים Element משלהם (`@spyglass`, `@headphones`, `@door`).
  אם חפץ מופיע ביותר משוט אחד וחשוב שייראה זהה — הוא צריך Element.
- **סטוריבורדים / סקיצות** הם Element לגיטימי — הם נועלים לוגיקה מרחבית או פרופורציות.
- **אודיו** יכול להיות Element (`@Audio`) — במדריך זה הקלטת voice-memo של הזמזום.

---

## מבנה פרומפט הוידאו

כל פרומפט סצנה בנוי מאותם בלוקים, באותו סדר:

| בלוק | תפקיד |
|------|-------|
| **כותרת** | `11 shots, 22 seconds total, 2.39:1, anamorphic. Hard cuts…` — כמה שוטים, כמה שניות, יחס |
| **SCENE ENGINE** | GOAL / OBSTACLE / TACTIC / REVERSAL / VALUE SHIFT — עמוד השדרה הדרמטי |
| **REFERENCE DEFINITIONS** | מה כל `@tag` אומר, במילים. "face and hair and wardrobe match the reference exactly in every frame… Reference." |
| **TECHNICAL BLOCK** | אופטיקה, גריידינג, פילם, שאטר. **זהה בכל הסצנות** — זה מה שמחבר שש תקופות לסרט אחד |
| **LAWS** | חוקי ברזל: LOCATION LOCK, FACE RULE, FACE STABILITY, PROPS, LEVEL CAMERA, LIVING CROWD… |
| **SHOT 1..N** | לכל שוט: טיים-קוד, סוג קאט, מיקום מצלמה, בלוקינג (`x≈70%`), פעולה |
| **ACTING TASK** | לכל דמות בשוט — **מה היא עושה**, לא מה היא מרגישה |
| **PHYSICS** | משקל, אינרציה, איך בד ושיער מתנהגים |
| **SFX** | פס קול דיאגטי בלבד. `No dialogue, no music.` |

### שלוש התובנות שעושות את ההבדל

1. **Acting task, לא רגש.**
   לא כותבים "הוא מאוהב". כותבים: *"task — understand what just happened to him;
   he checks both her eyes, one then the other, reading whether she noticed him noticing."*
   המודל יודע לביים משימה, הוא לא יודע לביים רגש.

2. **בלוקינג מספרי.**
   `enters frame-RIGHT (x≈70%) moving screen-left` — לא "משמאל". מספרים מונעים
   מהמודל להחליט לבד מי איפה.

3. **בלוק טכני זהה בכל הסצנות.**
   אותה אופטיקה, אותו גרעין, אותו גרייד. זה הדבק שהופך את שש התקופות לסרט אחד
   ולא לשישה קליפים.

---

## הגדרות גנרציה ב-Cinema Studio

| מה | מודל | יחס | רזולוציה | אורך |
|----|------|-----|----------|------|
| וידאו | Seedance 2.5 | 21:9 | 1080p | 25s / 30s |

הערה: הסצנה של הנמל היא 22 שניות בפועל אבל נוצרת בגנרציה של 30 שניות —
עודף מוותרים עליו בעריכה. סצנת הקרנבל מפוצלת ל-3 גנרציות של 15 שניות,
וסצנת הפיראטים ל-30s + 15s, כי רצף מלא לא נכנס בגנרציה אחת.
