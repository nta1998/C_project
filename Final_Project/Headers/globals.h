/*All the global values in the program*/
#ifndef FINAL_PROJECT_GLOBALS_H
#define FINAL_PROJECT_GLOBALS_H

/* The number of registers in the simulated computer $0-$31  */
#define NUM_OF_REG 32

/* Maximum length of a label in command line  */
#define MAX_LABEL_LENGTH 31

/* Maximum length of a single command line ('\n' include)  */
#define MAX_LINE_LENGTH 81

/* Default IC value - start address: 100 */
#define IC_INIT_VALUE 100

/* Define opcode length in bits */
#define OPCODE_BITS 6

/* For R,I type commandes */
#define REGISTER_FIELD_BITS 5
#define FUNCT_BITS 5
#define IMMEDIATE_BITS 16
#define IMMEDIATE_MIN (-32768)
#define IMMEDIATE_MAX (32767)

/* For J type commandes */
#define JTYPE_REG_BITS 1
#define JTYPE_ADDRESS_BITS 25

/* The number of all commandes */
#define COMMANDES_COUNT 27

/* The number of R type commandes */
#define COMMANDES_COUNT 8

/* The number of I type commandes */
#define COMMANDES_COUNT 15

/* The number of J type commandes */
#define COMMANDES_COUNT 4

/* The Size of Byte: 8 bits */
#define BYTE_SIZE 8

/* The minimum and maximum address range (25 bits unsigned) */
#define MAX_NUM ((1UL << JTYPE_ADDRESS_BITS) - 1)
#define MIN_NUM -((1UL << JTYPE_ADDRESS_BITS) - 1)

/* CHAEK! */
#define MAX_CODE_IMAGE_BYTES 4096
#define MAX_DATA_IMAGE_BYTES 4096

/* Ending files */
#define SOURCE_FILE_EXTENSION ".as"
#define EXPANDED_FILE_EXTENSION ".am"
#define OBJECT_FILE_EXTENSION ".ob"
#define ENTRIES_FILE_EXTENSION ".ent"
#define EXTERNALS_FILE_EXTENSION ".ext"

#endif
