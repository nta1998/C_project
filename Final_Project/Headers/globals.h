/*All the global values in the program*/
#ifndef GLOBALS_H
#define GLOBALS_H

/* Maximum length's of the program  */
#define MAX_LABEL_LEN 31
#define MAX_MCRO_LEN 31
#define MAX_NAME_LEN 31
#define MAX_COMMEND_NAME_LEN 7
#define MAX_REST_LEN 26
#define MAX_LINES 256
#define MAX_MCRO_LINES 50
#define KEY_WORDES 38
#define MAX_OPERAND_LEN 31
#define MAX_OPERANDS 50
#define QUESTION_MARK 63
#define HALF_BYTE 4
#define DEC_BASE 10

/* The number of registers in the simulated computer $0-$31  */
#define NUM_OF_REG 32
#define FIRST_REG 0
#define LAST_REG 31
#define RS 0
#define RT 1
#define RD 2

/* Maximum length of a single command line ('\n' include)  */
#define MAX_LINE_LEN 81

/* Commands line length in bits */
#define OPCODE_BITS 6
/* For R,I type commandes */
#define REG_FIELD_BITS 5
#define FUNCT_BITS 5
#define IMMEDIATE_BITS 16
#define UNUSE_BITS 6
/* For J type commandes */
#define JTYPE_REG_BITS 1
#define JTYPE_ADDRESS_BITS 25

/* The Size of valid byte's */
#define BYTE_SIZE 8
#define DB_BYTE_SIZE 1
#define ASCIZ_BYTE_SIZE 1
#define DH_BYTE_SIZE 2
#define DW_BYTE_SIZE 4

/* The range of valid number */
#define MAX_DB_OP 127L
#define MIN_DB_OP -128L
#define MAX_DH_OP 32767L
#define MIN_DH_OP -32768L
#define MAX_DW_OP 2147483647L
#define MIN_DW_OP -2147483648L

/* Valid adrass's */
#define IMMEDIATE_MIN -32768L
#define IMMEDIATE_MAX 32767L
/* Default IC value - start address: 100 */
#define START_ADDRESS 100
/* The minimum and maximum address range (25 bits unsigned) */
#define MAX_JUMP ((1UL << JTYPE_ADDRESS_BITS) - 1)
#define MIN_JUMP -((1UL << JTYPE_ADDRESS_BITS) - 1)

typedef enum {
    FALSE = 0,
    TRUE = 1
} Bool;

typedef struct {
    const char *file_name;
    int line_num;
    char *data;
} Line;

#endif 
