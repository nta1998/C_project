#ifndef MEMORY_IMAGE_H
#define MEMORY_IMAGE_H

#include "globals.h"

/**
 * A structure representing a line of code in the memory image.
 * @param address: an integer representing the memory address of the code line.
 * @param source_code: a string containing the original source code for the line.
 * @param machine_code: an unsigned long integer representing the machine code for the line.
*/

typedef struct {
    int address;
    char source_code[MAX_LINE_LEN+1];
    unsigned long machine_code;
} Code_line;

/**
 * A structure representing a line of data in the memory image.
 * @param address: an integer representing the memory address of the data line.
 * @param machine_code: an unsigned long integer representing the machine code for the data line.
 * @param byte_size: an integer representing the size of the data in bytes (1, 2, or 4).
*/

typedef struct {
    int address;
    unsigned long machine_code;
    int byte_size;
} Data_line;

/**
 * The function role is to reset the memory image.
 * It clears the code and data images, resets the instruction counter (IC) and data counter (DC),
 * and prepares the memory image for a new assembly process.
*/

void memory_image_reset(void);

/**
 * The function is called whenever a new line of code is processed.
 * It adds the line to the code image, storing its address, source code, and machine code.
 * @param source_code: a string containing the original source code for the line.
 * @param machine_code: an unsigned long integer representing the machine code for the line.
 * @return TRUE if the line was successfully added; FALSE if the code image is full.
*/

Bool add_code_line(const char *source_code, unsigned long machine_code);

/**
 * The function is called whenever a new line of data is processed.
 * It adds the line to the data image, storing its address and machine code.
 * @param machine_code: an unsigned long integer representing the machine code for the data line.
 * @param byte_size: an integer representing the size of the data in bytes (1, 2, or 4).
 * @return TRUE if the line was successfully added; FALSE if the data image is full.
*/

Bool add_data_line(unsigned long machine_code, int byte_size);

/**
 * The function returns the current instruction counter (IC) value.
 * @return an integer representing the current IC value.
*/

int get_IC(void);

/**
 * The function returns the current data counter (DC) value.
 * @return an integer representing the current DC value.
*/
int get_DC(void);

/**
 * The function returns a pointer to the code image array.
 * @return a pointer to the array of CODE_LINE structures representing the code image.
*/
const Code_line *get_code_image(void);

/**
 * The function returns a pointer to the data image array.
 * @return a pointer to the array of DATA_LINE structures representing the data image.
*/

/**
 * The function updates the machine code of an existing entry in the code image,
 * identified by its index. Used by second_pass to complete the encoding of
 * instructions whose operand depended on a forward-referenced label.
 * @param index: the index of the code line in the code image array to update.
 * @param machine_code: the new machine code value to store.
 * @return TRUE if the update succeeded; FALSE if index is out of range.
*/
Bool update_code_machine_code(int index, unsigned long machine_code);

const Data_line *get_data_image(void);

/**/
void data_image_table_shift_data(long final_ic);

/**/
void data_image_table_shift_data(long final_ic);

/**
 * The function returns the number of lines in the code image.
 * @return an integer representing the count of code lines.
*/

int get_IC_count(void);

/**
 * The function returns the number of lines in the data image.
 * @return an integer representing the count of data lines.
*/

int get_DC_count(void);

#endif
