#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

Bool is_instruction_word(const char *name);

/**
 * Function that checks whether the current name is reserved word.
 * @param name: pointer to the current name.
 * @return a Bool indicating whether the name is valid (TRUE) or not (FALSE).
*/
Bool is_reserved_word(const char *name);

/**
 * The function role is to perform the pre-assembly stage of the assembler.
 * It reads the input assembly file, processes macro definitions and calls,
 * and generates an output file with expanded macros.
 * @param as_file: input assembly file name.
 * @param am_file: output file name after macro expansion.
 * @return a Bool indicating success (TRUE) or failure (FALSE).
*/
Bool pre_assembler(const char *as_file, const char *am_file);

#endif

