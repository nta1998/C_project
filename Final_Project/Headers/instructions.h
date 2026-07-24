#ifndef FINAL_PROJECT_INSTRUCTIONS_H
#define FINAL_PROJECT_INSTRUCTIONS_H

#include <stddef.h>
#include "globals.h"

/*  
 *   OPERANDS_R_ARITH     - Arithmetic and Logical  add/sub/and/or/nor      3 register (rs, rt, rd)
 *   OPERANDS_R_COPY      - Copy                    move/mvhi/mvlo          2 register (rs=dest, rd=src)
 *   OPERANDS_I_ARITH     - Arithmetic and Logical  addi/subi/andi/ori/nori 2 register (rs, rt)
 *   OPERANDS_I_MEM       - Memore                  lb/sb/lw/sw/lh/sh       2 register (rs=Mem[rt+immed])
 *   OPERANDS_I_BRANCH    - Conditional branching   bne/beq/blt/bgt         2 register (rs, rt)
 *   OPERANDS_J_JUMP      - jmp label or one register
 *   OPERANDS_J_LABEL     - la/call label only
 *   OPERANDS_J_NONE      - hlt 
 */
typedef enum InstClass{
  R_ARITH, 
  R_COPY, 
  I_ARITH,
  I_BRANCH, 
  I_MEM, 
  J_TYPE
}InstClass;

/*  
 *   name     - the name of the instruction  
 *   cls      - the instruction type InstClass                 
 *   opcode   - Unique code for Identification
 *   funct    - Unique code for Identification (Only relevant in instructions of type R)
 */
typedef struct {
  const char *name;
  InstClass cls;
  int opcode;
  int funct;
} InstInfo;

/**
 * This function find the info for a commend 
 * @param name a pointer of char the commend name we need to find
 * @return a InstInfo object or NULL if not found 
 */
const InstInfo *inst_find(const char *name);

#endif
