#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "globals.h"

/**
 * An INSTRUCTION_TYPE enumeration.
 * @param R_A_TYPE: Represents R-type: arithmetic instructions.
 * @param R_C_TYPE: Represents R-type: copy instructions.
 * @param I_A_TYPE: Represents I-type: arithmetic instructions.
 * @param I_B_TYPE: Represents I-type: branch instructions.
 * @param I_M_TYPE: Represents I-type: memory instructions.
 * @param J_TYPE: Represents J-type instructions.
*/
typedef enum {
  R_A_TYPE, 
  R_C_TYPE, 
  I_A_TYPE,
  I_B_TYPE, 
  I_M_TYPE, 
  J_TYPE
} INSTRUCTION_TYPE;

/**
 * An INSTRUCTION_INFO data structure.
 * @param name: a string for an instruction name.
 * @param type: an INSTRUCTION_TYPE indicating the type of the instruction.
 * @param opcode: an integer representing the opcode of the instruction.
 * @param funct: an integer representing the function code of the instruction (if applicable).
 * @param num_operands: an integer indicating the number of operands required by the instruction.
*/
typedef struct {
  const char *name;
  INSTRUCTION_TYPE type;
  int opcode;
  int funct;
  int num_operands;
} INSTRUCTION_INFO;

/**
 * Searches the instruction table for an instruction by name.
 * @param curr_name: the instruction name to search for.
 * @return a pointer to the matching INSTRUCTION_INFO if found, or NULL if not found.
*/
INSTRUCTION_INFO *instruction_search(const char *curr_name);

#endif
