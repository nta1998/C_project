#ifndef PARSER_H
#define PARSER_H

#include "globals.h"
#include "instructions.h"

/* Line types in an input file */
typedef enum LineKind{
  LINE_INVALID, /* Invalid line */ 
  LINE_EMPTY, /* Whitespace line */ 
  LINE_COMMENT, /* Comment line */ 
  LINE_DIRECTIVE, /* Directive line */ 
  LINE_INSTRUCTION /* Instruction line */ 
}LINE_KIND;

/* Definition of a structure of type 'LINE_KIND' */
typedef struct Parsed_line{
  LINE_KIND kind;
  const Instruction_info *instruction;
  char label[MAX_LABEL_LEN + 1];
  char name[MAX_COMMEND_NAME_LEN + 1];
  char rest[MAX_REST_LEN + 1];
} Parsed_line;

/**
 

 */
Bool split_line(Line line, Parsed_line *curr_line);
Bool is_valid_rest_split(Line line, Parsed_line *curr_line, char ops[MAX_OPERANDS][MAX_OPERAND_LEN], long values_out[], int *count_out);

#endif