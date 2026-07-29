
/* output_writer.c */
/**
 * @file output.c
 * @brief This file contains the implementation of the output-writing functions,
 * which produce the .ob, .ent, and .ext files after a successful assembly.
*/

#include <stdio.h>

#include "globals.h"
#include "memory_image.h"
#include "symbol_table.h"
#include "external_table.h"
#include "output.h"

/**
 * A static function that writes a single row of the object file - a decimal address followed by
 * up to 4 space-separated hexadecimal bytes.
 * @param file: the already-open object file to write to.
 * @param address: the decimal address to print at the start of the row.
 * @param row: an array holding the raw bytes to print for this row.
 * @param row_len: the number of valid bytes in row (1 to 4).
*/

static void flush_row(FILE *file, int address, const unsigned char *row, int row_len){
    int i;
    fprintf(file, "%04d", address);
    for (i = 0; i < row_len; i++){
        fprintf(file, " %02X", row[i]);
    }
    fprintf(file, "\n");
}

/**
 * A static function that writes the object file: a header line with the total code and data
 * lengths in bytes, followed by the flattened code and data bytes, split
 * into rows of up to 4 bytes each in little-endian order.
 * @param ob_file: the full path of the object file to create.
*/

static void write_object_file(const char *ob_file){
    FILE *file;
    const CODE_LINE *code = get_code_image();
    const DATA_LINE *data = get_data_image();
    int code_count = get_IC_count();
    int data_count = get_DC_count();
    int address = START_ADDRESS;
    unsigned char row[4];
    int row_len = 0;
    int i, b;

    file = fopen(ob_file, "w");
    if (file == NULL){
        return;
    }

    fprintf(file, "%d %d\n", get_IC() - START_ADDRESS, get_DC());

    for (i = 0; i < code_count; i++){
        /** זה מה שהלולאה עושה בעצם - עובדת לפי השיטה של littel endian
         * unsigned char byte0 = (machine_code >> 0)  & 0xFF;   / b=0, b*8=0  /
         * unsigned char byte1 = (machine_code >> 8)  & 0xFF;   / b=1, b*8=8  /
         * unsigned char byte2 = (machine_code >> 16) & 0xFF;   / b=2, b*8=16 /
         * unsigned char byte3 = (machine_code >> 24) & 0xFF;   / b=3, b*8=24 /
         */
        for (b = 0; b < 4; b++){
            row[row_len] = (code[i].machine_code >> (b * 8)) & 0xFF;
            row_len++;
            if (row_len == 4){
                flush_row(file, address, row, row_len);
                address += 4;
                row_len = 0;
            }
        }
    }

    for (i = 0; i < data_count; i++){
        for (b = 0; b < data[i].byte_size; b++){
            row[row_len] = (data[i].machine_code >> (b * 8)) & 0xFF;
            row_len++;
            if (row_len == 4){
                flush_row(file, address, row, row_len);
                address += 4;
                row_len = 0;
            }
        }
    }

    if (row_len > 0){
        flush_row(file, address, row, row_len);
    }

    fclose(file);
}

/**
 * A static function that writes the entries file: one line per symbol marked as an entry
 * point, containing its name and decimal value. If no symbol is marked
 * as an entry, the file is removed again after being created.
 * @param ent_file: the full path of the entries file to create.
*/

static void write_entries_file(const char *ent_file){
    FILE *file;
    Symbol *curr;
    Bool any_entries = FALSE;

    file = fopen(ent_file, "w");
    if (file == NULL){
        return;
    }

    curr = symbol_table_get_head();
    while (curr != NULL){
        if (curr->is_entry){
            fprintf(file, "%s %04ld\n", curr->symbol_name, curr->value);
            any_entries = TRUE;
        }
        curr = curr->next;
    }

    fclose(file);
    if (!any_entries){
        remove(ent_file);
    }
}

/**
 * A static function that writes the externals file: one line per recorded usage of an
 * external symbol, containing its name and the decimal address where it
 * was used. If no usages were recorded, the file is removed again after
 * being created.
 * @param ext_file: the full path of the externals file to create.
*/

static void write_externals_file(const char *ext_file){
    FILE *file;
    External *curr;
    Bool any_externals = FALSE;

    file = fopen(ext_file, "w");
    if (file == NULL){
        return;
    }

    curr = external_table_get_head();
    while (curr != NULL){
        fprintf(file, "%s %04ld\n", curr->symbol_name, curr->address);
        any_externals = TRUE;
        curr = curr->next;
    }

    fclose(file);
    if (!any_externals){
        remove(ext_file);
    }
}

void write_output_files(const char *ob_file, const char *ent_file, const char *ext_file){
    write_object_file(ob_file);
    write_entries_file(ent_file);
    write_externals_file(ext_file);
}
