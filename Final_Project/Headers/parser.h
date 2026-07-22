#ifndef FINAL_PROJECT_PARSER_H
#define FINAL_PROJECT_PARSER_H

#include <stddef.h>
#include <stdio.h>
#include "globals.h"

/* Line types in an input file */
typedef enum LineKind {
  LINE_EMPTY, /* Whitespace line */ 
  LINE_COMMENT, /* Comment line */ 
  LINE_DIRECTIVE, /* Directive line */ 
  LINE_INSTRUCTION, /* Instruction line */ 
  LINE_INVALID /* Invalid line */ 
};

/* Definition of a structure of type 'LineKind' */
typedef struct {
  LineKind kind;
  char label[MAX_LABEL_LENGTH + 1];
  char name[MAX_LABEL_LENGTH + 1];
  char rest[MAX_LINE_LENGTH + 1];
} ParsedLine;

/**
 * This function copies the content of a macro
 * @param raw row pointer to a row in the source file
 * @param out a pointer of ParsedLine type 
 * @param file a pointer to file name 
 * @param ln the line number 
 * @return a Bool 
 */
Bool line_split(const char *raw, ParsedLine *out, const char *file, int ln);

/**
 * This function copies the content of a macro
 * @param s row pointer to a row in the source file
 * @return a Bool 
 */
Bool is_valid_label(const char *s);

/**
 * This function copies the content of a macro
 * @param s row pointer to a row in the source file
 * @return a Bool 
 */
Bool is_reserved_word(const char *s);

/**
 * This function copies the content of a macro
 * @param s row pointer to a row in the source file
 * @param out a pointer of ParsedLine type 
 * @return a Bool 
 */
Bool parse_number(const char *s, long *out); 

/**
 * This function copies the content of a macro
 * @param raw row pointer to a row in the source file
 * @param out a pointer of ParsedLine type 
 * @param file a pointer to t
 * @param ln a pointer to the line number from which we are reading the 
 * @param ln a pointer to the line number from which we are reading the 
 * @return a Bool 
 */
int  operands_split(char *rest, char *ops[], int max_ops, const char *file, int ln);

/**
 * This function copies the content of a macro
 * @param raw row pointer to a row in the source file
 * @param out a pointer of ParsedLine type 
 * @param file a pointer to t
 * @param ln a pointer to the line number from which we are reading the 
 * @return a Bool 
 */
int  parse_register(const char *s);
#endif