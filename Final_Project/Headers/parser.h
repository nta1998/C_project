#ifndef FINAL_PROJECT_PARSER_H
#define FINAL_PROJECT_PARSER_H

#include <stddef.h>
#include <stdio.h>
#include <stdio.h>
#include "globals.h"
#include "instructions.h"


/* Line types in an input file */
typedef enum LineKind{
  LINE_INVALID, /* Invalid line */ 
  LINE_EMPTY, /* Whitespace line */ 
  LINE_COMMENT, /* Comment line */ 
  LINE_DIRECTIVE, /* Directive line */ 
  LINE_INSTRUCTION /* Instruction line */ 
}LineKind;

/* Definition of a structure of type 'LineKind' */
typedef struct Parsed_line{
  LineKind kind;
  const Instruction_info *instruction;
  char label[MAX_LABEL_LEN + 1];
  char name[MAX_COMMEND_NAME_LEN + 1];
  char rest[MAX_REST_LEN + 1];
  /*add error*/
} Parsed_line;

/**
 * This function split the raw row into a ParsedLine
 * @param raw a pointer to a raw row from the source file
 * @param out a pointer of ParsedLine type 
 * @param file a pointer to the source file
 * @param ln the line number of the row in the source file
* @return a Bool TRUE/FALSE
 */
Bool line_split(const char *raw, Parsed_line *out, Line line);

/**
 * This function check if the given string is a valid label 
 * a valid label start with a letter (a-z/A-Z) and contain only letters and numbers
 * and not longer than MAX_LABEL_LENGTH
 * and end with a ':' char without any white spaces before it.
 * @param s row pointer to a string
 * @return a Bool TRUE/FALSE
 */
Bool is_valid_label(char *s,Line line);




/**
 * This function split the operands part of a line into an array of strings
 * @param rest a pointer to the operands part of a line
 * @param out a pointer to an array to store the operands pointers 
 * @param max_ops the maximum number of operands
 * @param file a pointer to the source file
 * @param ln the line number of the row in the source file
 * @return number of operands found or -1 if error
 */
int operands_split(Parsed_line *rest, char ops[][81],Line line);

Bool is_directive_word(const char *s);
void skip_white_space(const char *raw, int *i);
void kind_test(Parsed_line *out,Line line);
void splid_rest(const char *raw, Parsed_line *out ,int *i);
void splid_name(const char *raw, Parsed_line *out ,int *i,Line line);
void splid_label(const char *raw, Parsed_line *out ,int *i,Line line);

#endif
 