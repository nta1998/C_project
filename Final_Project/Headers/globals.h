/*All the global values in the program*/
#ifndef GLOBALS_H
#define GLOBALS_H

/* The number of registers in the simulated computer $0-$31  */
#define NUM_OF_REG 32

/* Maximum length of a label or mcro name in command line  */
#define MAX_LABEL_LEN 31
#define MAX_MCRO_LEN 31

/* Maximum length of a single command line ('\n' include)  */
#define MAX_LINE_LEN 81

/* Maximum length of a single name */
#define MAX_NAME_LEN 7

/* Maximum length of a rest */
#define MAX_REST_LEN 26

/* Default IC value - start address: 100 */
#define START_ADDRESS 100

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
#define COMMANDES_R_COUNT 8

/* The number of I type commandes */
#define COMMANDES_I_COUNT 15

/* The number of J type commandes */
#define COMMANDES_J_COUNT 4

/* The Size of Byte: 8 bits */
#define BYTE_SIZE 8
#define DB_BYTE_SIZE 1
#define DH_BYTE_SIZE 2
#define DW_BYTE_SIZE 4

/* The minimum and maximum address range (25 bits unsigned) */
#define MAX_NUM ((1UL << JTYPE_ADDRESS_BITS) - 1)
#define MIN_NUM -((1UL << JTYPE_ADDRESS_BITS) - 1)

/* CHAEK! */
#define MAX_CODE_IMAGE_BYTES 4096
#define MAX_DATA_IMAGE_BYTES 4096
#define MAX_LINES 256
#define MAX_MCRO_LINES 50

/* Ending files */
#define SOURCE_FILE_EXTENSION ".as"
#define EXPANDED_FILE_EXTENSION ".am"
#define OBJECT_FILE_EXTENSION ".ob"
#define ENTRIES_FILE_EXTENSION ".ent"
#define EXTERNALS_FILE_EXTENSION ".ext"

typedef enum {
    FALSE = 0,
    TRUE = 1
} Bool;

typedef struct {
    const char *file_name;
    int line_num;
    char *data;
} Line;

#define WHITE_SPACE ' '

#endif 
