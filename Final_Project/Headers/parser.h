#ifndef PARSER_H
#define PARSER_H

#include "globals.h"
#include "instructions.h"

/** Line types in an input file :
 * @param LINE_INVALID:Line that failed parsing/validation.
 * @param LINE_EMPTY:Blank or whitespace-only line.
 * @param LINE_COMMENT:Line starting with ';'.
 * @param LINE_DIRECTIVE:Line starting with a directive (.db, .dh, .dw, .asciz, .entry, .extern).
 * @param LINE_INSTRUCTION:Line starting with a recognized instruction opcode.
 */
 
typedef enum {
  LINE_INVALID,
  LINE_EMPTY,
  LINE_COMMENT, 
  LINE_DIRECTIVE, 
  LINE_INSTRUCTION 
} LINE_KIND;

/** Holds the result of splitting/parsing a single source line into its components :
 * @param kind:Category of the line (empty, comment, directive, instruction).
 * @param instruction:Matching instruction info, set only when kind == LINE_INSTRUCTION.
 * @param label:Label defined at the start of the line, if any (without the trailing ':'.
 * @param name:Directive or instruction name (e.g. ".db", "mov").
 * @param rest:Remainder of the line after the name, holding the raw operand text.
 */ 

typedef struct Parsed_line{
  LINE_KIND kind; 
  const Instruction_info *instruction;
  char label[MAX_LABEL_LEN + 1];
  char name[MAX_COMMEND_NAME_LEN + 1];
  char rest[MAX_REST_LEN + 1]; 
} Parsed_line;

/**
 * Splits a raw source line into a Parsed_line: identifies its kind (empty,
 * comment, directive or instruction), and extracts the label, name and
 * the remaining operand text.
 */
Bool split_line(Line line, Parsed_line *curr_line);

/**
 * Validates and splits the operand text stored in curr_line->rest, based on
 * whether the line is a directive or an instruction. On success, fills
 * either 'ops' (instruction operands) or 'values_out' (directive numeric
 * operands) and sets *count_out to the number of operands found. Also
 * validates the label name, if the line defines one.
 */
Bool is_valid_rest_split(Line line, Parsed_line *curr_line, char ops[MAX_OPERANDS][MAX_OPERAND_LEN], long values_out[], int *count_out);

#endif
