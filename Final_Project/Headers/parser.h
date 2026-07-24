#ifndef FINAL_PROJECT_PARSER_H
#define FINAL_PROJECT_PARSER_H

#include <stddef.h>
#include <stdio.h>
#include "globals.h"

/* Line types in an input file */
typedef enum LineKind{
  LINE_EMPTY, /* Whitespace line */ 
  LINE_COMMENT, /* Comment line */ 
  LINE_DIRECTIVE, /* Directive line */ 
  LINE_INSTRUCTION, /* Instruction line */ 
  LINE_INVALID /* Invalid line */ 
}LineKind;

/* Definition of a structure of type 'LineKind' */
typedef struct {
  LineKind kind;
  char label[MAX_LABEL_LENGTH + 1];
  char name[MAX_LABEL_LENGTH + 1];
  char rest[MAX_LINE_LENGTH + 1];
} ParsedLine;

/**
 * This function split the raw row into a ParsedLine
 * @param raw a pointer to a raw row from the source file
 * @param out a pointer of ParsedLine type 
 * @param file a pointer to the source file
 * @param ln the line number of the row in the source file
* @return a Bool TRUE/FALSE
 */
Bool line_split(const char *raw, ParsedLine *out, const char *file, int ln);

/**
 * This function check if the given string is a valid label 
 * a valid label start with a letter (a-z/A-Z) and contain only letters and numbers
 * and not longer than MAX_LABEL_LENGTH
 * and end with a ':' char without any white spaces before it.
 * @param s row pointer to a string
 * @return a Bool TRUE/FALSE
 */
Bool is_valid_label(const char *s);

/**
 * This function check if the given string is a reserved word 
 * reserved words are operands types (R,I,J) 
 * @param s a pointer to a string we need to check
 * @return a Bool TRUE/FALSE
 */
Bool is_reserved_word(const char *s);

/**
 * This function check if the given string is a valid number (whole numbers positive or negative only) 
 * @param s a pointer to a string we need to check
 * @param out a pointer of long type 
 * @return a Bool TRUE/FALSE
 */
Bool parse_number(const char *s, long *out); 

/**
 * This function split the operands part of a line into an array of strings
 * @param rest a pointer to the operands part of a line
 * @param out a pointer to an array to store the operands pointers 
 * @param max_ops the maximum number of operands
 * @param file a pointer to the source file
 * @param ln the line number of the row in the source file
 * @return number of operands found or -1 if error
 */
int  operands_split(char *rest, char *ops[], int max_ops, const char *file, int ln);

/**
 * This function parse a register name and return the register number
 * @param s a pointer to a string we need to parse
 * @return the register number or -1 if error
 */
int  parse_register(const char *s);

#endif
 