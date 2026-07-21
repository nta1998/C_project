#ifndef FINAL_PROJECT_PARSER_H
#define FINAL_PROJECT_PARSER_H

#include <stddef.h>
#include "globals.h"

/* Line types in an input file */
typedef enum {
  LINE_EMPTY, /* Whitespace line */ 
  LINE_COMMENT, /* Comment line */ 
  LINE_DIRECTIVE, /* Directive line */ 
  LINE_INSTRUCTION, /* Instruction line */ 
  LINE_INVALID /* Invalid line */ 
} LineKind;

/* Definition of a structure of type 'LineKind' */
typedef struct {
  LineKind kind;
  char label[MAX_LABEL_LEN + 1];
  char name[MAX_LINE_LEN + 1];
  char rest[MAX_LINE_LEN + 1];
} ParsedLine;

#endif


Bool line_split(const char *raw, ParsedLine *out, const char *file, int ln);
int  operands_split(char *rest, char *ops[], int max_ops, const char *file, int ln);
Bool is_valid_label(const char *s);
Bool is_reserved_word(const char *s);
int  parse_register(const char *s);
Bool parse_number(const char *s, long *out);
