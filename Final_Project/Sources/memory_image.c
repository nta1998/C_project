/**
 * @file memory_image.c
 * @brief This file contains the implementation of the memory image functions, which manage the code and 
 * data images for the assembler.
 * The memory image consists of two separate arrays: one for code lines and another for data lines. 
*/

#include <string.h>
#include <stdlib.h>

#include "../Headers/memory_image.h"

/* Static variables (limited to this file only). */

static CODE_LINE code_image[MAX_LINES];     /* Array to store code lines in the memory image. */
static int IC_address = START_ADDRESS;      /* Current instruction counter (IC) address. Start at 100. */
static int IC_counter = 0;                  /* The number of code lines added to the code image. */

static DATA_LINE data_image[MAX_LINES];     /* Array to store data lines in the memory image. */
static int DC_address = 0;                  /* Current data counter (DC) address. */
static int DC_counter = 0;                  /* The number of data lines added to the data image. */

void memory_image_reset(void){
    IC_address = START_ADDRESS;
    DC_address = 0;
    IC_counter = 0;
    DC_counter = 0;
}

Bool add_code_line(const char *source_code, unsigned long machine_code){
    if (IC_counter >= MAX_LINES){
        return FALSE;
    }
    code_image[IC_counter].address = IC_address;
    strcpy(code_image[IC_counter].source_code, source_code);
    code_image[IC_counter].machine_code = machine_code;
    IC_counter++;
    IC_address += 4;
    return TRUE;
}

Bool add_data_line(unsigned long machine_code, int byte_size){
    if (DC_counter >= MAX_LINES){
        return FALSE;
    }
    data_image[DC_counter].address = DC_address;
    data_image[DC_counter].machine_code = machine_code;
    data_image[DC_counter].byte_size = byte_size;
    DC_counter++;
    DC_address += byte_size;
    return TRUE;
}

int get_IC(void){
    return IC_address;
}

int get_DC(void){
    return DC_address;
}

const CODE_LINE *get_code_image(void){
    return code_image;
}

const DATA_LINE *get_data_image(void){
    return data_image;
}

int get_IC_count(void){
    return IC_counter;
}

int get_DC_count(void){
    return DC_counter;
}