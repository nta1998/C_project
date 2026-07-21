#ifndef FINAL_PROJECT_INSTRUCTIONS_H
#define FINAL_PROJECT_INSTRUCTIONS_H

#include <stddef.h>
#include "globals.h"

/* תבנית האופרנדים הצפויה בשורת המקור, לפי סוגי הפקודות שהוגדרו במסמך:
 *   OPERANDS_R_ARITH    - שלושה רגיסטרים: add/sub/and/or/nor          (rs, rt, rd)
 *   OPERANDS_R_COPY     - שני רגיסטרים: move/mvhi/mvlo                 (rs=dest, rd=src - ראו הערה במימוש)
 *   OPERANDS_REG_IMM_REG- רגיסטר, מיידי, רגיסטר: addi/subi/andi/ori/nori
 *                          *וגם* lb/sb/lw/sw/lh/sh - מבחינת מבנה
 *                          האופרנדים והשדות (rs=אופרנד1, immed=אופרנד2,
 *                          rt=אופרנד3) שתי הקבוצות זהות לחלוטין; ההבדל
 *                          ביניהן הוא סמנטי בלבד (תוצאה אריתמטית מול
 *                          כתובת בזיכרון), ולכן קידוד משותף להן חוסך שכפול קוד.
 *   OPERANDS_I_BRANCH   - רגיסטר, רגיסטר, תווית: bne/beq/blt/bgt
 *   OPERANDS_J_JUMP     - תווית *או* רגיסטר יחיד: jmp
 *   OPERANDS_J_LABEL    - תווית יחידה בלבד: la/call
 *   OPERANDS_NONE       - ללא אופרנדים: hlt
 */
typedef enum {
  R_ARITH, 
  R_COPY, 
  I_ARITH,
  I_BRANCH, 
  I_MEM, 
  J_TYPE
  /*OPERANDS_NONE*/
} InstClass;

typedef struct {
  const char *name;
  InstClass cls;
  int opcode;
  int funct;
} InstInfo;

/* מחפשת פקודה לפי שם מילת הפעולה. מחזירה מצביע לרשומה הקבועה בטבלה, או
 * NULL אם השם אינו מילת פעולה מוכרת. */
const InstInfo *inst_find(const char *name);

#endif
