/* fp_main.c - דרייבר בדיקה עצמאי ל-first_pass.
 * מדפיס את טבלת הסמלים, את תמונת הקוד ואת תמונת הנתונים,
 * כולל ייצוג בינארי ופירוק לשדות של כל מילת מכונה.
 * אינו חלק מקובץ ההרצה 'assembler'. */

#include <stdio.h>
#include "../Headers/globals.h"
#include "../Headers/first_pass.h"
#include "../Headers/errors.h"
#include "../Headers/symbol_table.h"
#include "../Headers/memory_image.h"

static void print_binary(unsigned long value, int bits);
static void print_fields(unsigned long mc);
static void bitfield_sanity_check(void);

int main(int argc, char *argv[])
{
    FILE *am; int icf = 0, dcf = 0; Bool ok; Symbol *s; int i;
    const CODE_LINE *code; const DATA_LINE *data;

    if (argc < 2) { printf("usage: %s file.am\n", argv[0]); return 1; }
    am = fopen(argv[1], "r");
    if (am == NULL) { printf("cannot open %s\n", argv[1]); return 1; }

    bitfield_sanity_check();

    err_reset(); symbol_table_reset(); memory_image_reset();
    ok = first_pass(am, argv[1], NULL, NULL, NULL, &icf, &dcf);
    fclose(am);

    printf("\n=== first_pass returned %s ===\n", ok ? "TRUE" : "FALSE");
    printf("ICF = %d   DCF = %d\n", icf, dcf);

    printf("\n--- symbol table ---\n");
    for (s = symbol_table_get_head(); s != NULL; s = s->next)
        printf("  %-32s value=%-6ld attr=%d entry=%d\n",
               s->symbol_name, s->value, s->attribute, s->is_entry);

    printf("\n--- code image (%d lines) ---\n", get_IC_count());
    printf("  addr  hex       binary                                 fields\n");
    code = get_code_image();
    for (i = 0; i < get_IC_count(); i++) {
        printf("  %04d  %08lX  ", code[i].address, code[i].machine_code);
        print_binary(code[i].machine_code, 32);
        printf("  ");
        print_fields(code[i].machine_code);
        printf("\n                      %s\n", code[i].source_code);
    }

    printf("\n--- data image (%d lines) ---\n", get_DC_count());
    printf("  addr  hex       binary\n");
    data = get_data_image();
    for (i = 0; i < get_DC_count(); i++) {
        printf("  %04d  %08lX  ", data[i].address, data[i].machine_code);
        print_binary(data[i].machine_code, data[i].byte_size * 8);
        printf("  (%d byte%s)\n", data[i].byte_size,
               data[i].byte_size == 1 ? "" : "s");
    }

    print_summary_err();
    return ok ? 0 : 1;
}

/* מדפיס את 'bits' הביטים הנמוכים של value, מהמשמעותי לפחות משמעותי,
 * עם רווח כל 4 ביטים לקריאות. */
static void print_binary(unsigned long value, int bits)
{
    int i;
    for (i = bits - 1; i >= 0; i--) {
        putchar( ((value >> i) & 1UL) ? '1' : '0' );
        if (i % 4 == 0 && i != 0) putchar(' ');
    }
}

/* מפרק מילת מכונה לשדות שלה.
 * סוג ההוראה נקבע לפי ה-opcode (ביטים 31-26), לפי טבלת ההוראות:
 *   0-1    R   (R_A_TYPE / R_C_TYPE)
 *   10-24  I   (I_A / I_B / I_M)
 *   30+    J   (J_TYPE) */
static void print_fields(unsigned long mc)
{
    unsigned int opcode = (unsigned int)((mc >> 26) & 0x3FUL);

    if (opcode <= 1) {
        union machine_code_R u;
        u.all_in_one = mc;
        printf("[R] op=%-2u rs=%-2u rt=%-2u rd=%-2u funct=%u",
               opcode, u.fields.rs, u.fields.rt, u.fields.rd, u.fields.funct);
    }
    else if (opcode >= 30) {
        union machine_code_J u;
        u.all_in_one = mc;
        printf("[J] op=%-2u reg=%u addr=%u",
               opcode, u.fields.reg, u.fields.address);
    }
    else {
        union machine_code_I u;
        u.all_in_one = mc;
        /* immed מוצהר unsigned:16 - המרה ל-short מחזירה את הסימן */
        printf("[I] op=%-2u rs=%-2u rt=%-2u immed=%d",
               opcode, u.fields.rs, u.fields.rt, (int)(short)u.fields.immed);
    }
}

/* סדר שדות ביטים ב-C הוא מוגדר-מימוש. הבדיקה מוודאת ש-opcode אכן
 * יושב בביטים 31-26 לפני שסומכים על הפירוק שלמעלה. */
static void bitfield_sanity_check(void)
{
    union machine_code_R t;
    t.all_in_one = 0;
    t.fields.opcode = 0x3F;

    printf("bitfield check: opcode=0x3F -> %08lX  (expected FC000000)\n",
           t.all_in_one);
    printf("sizeof(union machine_code_R) = %lu bytes\n",
           (unsigned long)sizeof(union machine_code_R));

    if (t.all_in_one != 0xFC000000UL)
        printf("  !! סדר שדות הביטים אינו כצפוי - הפירוק למטה לא אמין\n");
}
