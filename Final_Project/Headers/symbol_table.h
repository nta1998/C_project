#ifndef FINAL_PROJECT_SYMBOL_TABLE_H
#define FINAL_PROJECT_SYMBOL_TABLE_H

#include "globals.h"

typedef enum {
    SYMBOL_CODE,
    SYMBOL_DATA,
    SYMBOL_EXTERNAL
} Attribute;

typedef struct {
    char name[MAX_LABEL_LENGTH + 1];
    long value;
    Attribute attribute;
    Boolean is_entry; /* TRUE אם הסמל הוצהר גם עם .entry */
    struct Symbol *next;
} Symbol;

typedef struct {
    Symbol *head;
} SymbolTable;



SymbolTable *symtab_create(void);
Symbol *symtab_find(SymbolTable *table, const char *name);
Symbol *symtab_add(SymbolTable *table, const char *name, long value, Attribute attribute);
Boolean symbol_table_mark_entry(SymbolTable *table, const char *name);
void symtab_add_icf_to_data(SymbolTable *table, long icf);
void symtab_free(SymbolTable *table);
Boolean define_new_symbol(SymbolTable *table, const char *name, long value, SymbolType type, int line_number);

typedef struct ExternalRef {
    char symbol_name[MAX_LABEL_LENGTH + 1];
    long address;
    struct ExternalRef *next;
} ExternalRef;

typedef struct {
    ExternalRef *head;
} ExternalRefList;

void external_ref_list_init(ExternalRefList *list);

/* מוסיפה רשומה חדשה: "הסמל symbol_name נדרש בכתובת address". רשומה אחת
 * לכל הופעה - אם אותו סמל נעשה בו שימוש פעמיים, יתווספו שתי רשומות. */
void external_ref_list_add(ExternalRefList *list, const char *symbol_name, long address);

void external_ref_list_free(ExternalRefList *list);

#endif /* SYMBOL_TABLE_H */
