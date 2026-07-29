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
} CODE_LINE;

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
} DATA_LINE;

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

const CODE_LINE *get_code_image(void);

/**
 * The function returns a pointer to the data image array.
 * @return a pointer to the array of DATA_LINE structures representing the data image.
*/

const DATA_LINE *get_data_image(void);

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