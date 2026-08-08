/**
 * @file second_pass.c
 * @brief This file role is to perform the second pass stage of the assembler.
 */

#include <stdio.h>
#include <string.h>

#include "../Headers/second_pass.h"
#include "../Headers/memory_image.h"
#include "../Headers/symbol_table.h"
#include "../Headers/external_table.h"
#include "../Headers/errors.h"
#include "../Headers/instructions.h"
#include "../Headers/parser.h"

#define IMMEDIATE_FIELD_MASK   0xFFFFUL
#define ADDRESS_FIELD_MASK 0x1FFFFFFUL

static unsigned long encode_word(unsigned long existing_word, long value, unsigned long field_mask){
    return (existing_word & ~field_mask) | ((unsigned long)value & field_mask);
}

Bool second_pass(const char *file_name){
    FILE *origin_file;
    Line curr_line;
    Parsed_line parsed_line;
    Symbol *curr_symbol;
    const Instruction_info *curr_instruction;
    char line[MAX_LINE_LEN+1];
    const Code_line *code_image;
    int curr_code_id;
    const char *label_name;
    int distance;
    unsigned long curr_encode_word;

    curr_line.file_name = file_name;
    curr_line.line_num = 0;
    origin_file = fopen(file_name, "r");

    if (origin_file == NULL){
        return FALSE;
    }

    code_image = get_code_image();
    curr_code_id = 0;

    while (fgets(line, sizeof(line), origin_file) != NULL) {
        curr_line.line_num ++;
        curr_line.data = line;
        line_split(line, &parsed_line, curr_line);

        if (parsed_line.kind == LINE_INVALID ||
            parsed_line.kind == LINE_EMPTY || 
            parsed_line.kind == LINE_COMMENT) {
            continue;
        }

        if (parsed_line.kind == LINE_DIRECTIVE) {
            if (strcmp(parsed_line.name, ".entry") == 0) {
                curr_symbol = symbol_search(parsed_line.rest);
                if (curr_symbol == NULL) {
                    err_report(curr_line, ERR_CODE_30);
                }
                else if (curr_symbol->attribute == S_EXTERNAL) {
                    err_report(curr_line, ERR_CODE_31);
                }
                else {
                    curr_symbol->is_entry = TRUE;
                }
            }
            continue;
        }

        curr_instruction = instruction_search(parsed_line.name);

        if (curr_instruction == NULL) {
            continue;
        }
        
        if ((curr_instruction->type == J_TYPE) && (strcmp(curr_instruction->name, "hlt") != 0)) {
            if (parsed_line.rest[0] != '$'){
                label_name = parsed_line.rest;
                curr_symbol = symbol_search(label_name);
                if(curr_symbol == NULL) {
                    err_report(curr_line, ERR_CODE_30);
                }
                else if (curr_symbol->attribute == S_EXTERNAL)
                {
                    if ((code_image[curr_code_id].machine_code & ADDRESS_FIELD_MASK) == '?')
                    {
                        curr_encode_word = encode_word(code_image[curr_code_id].machine_code, 0, ADDRESS_FIELD_MASK);
                        update_code_machine_code(curr_code_id, curr_encode_word);
                        external_add(curr_symbol->symbol_name, code_image[curr_code_id].address);    
                    }
                    else
                    {
                        external_add(curr_symbol->symbol_name, code_image[curr_code_id].address);
                    }
                }
                else if ((code_image[curr_code_id].machine_code & ADDRESS_FIELD_MASK) == '?')
                {
                    curr_encode_word = encode_word(code_image[curr_code_id].machine_code, curr_symbol->value, ADDRESS_FIELD_MASK);
                    update_code_machine_code(curr_code_id, curr_encode_word);
                }
            }
        }

        if (curr_instruction->type == I_B_TYPE){
            label_name = strrchr(parsed_line.rest, ',')+1;
            while (*label_name == ' ') {
                label_name++;
            }
            curr_symbol = symbol_search(label_name);
            if (curr_symbol == NULL){
                err_report(curr_line, ERR_CODE_30);
            }
            else {
                if (curr_symbol->attribute == S_EXTERNAL){
                    err_report(curr_line, ERR_CODE_33);
                }
                else {
                    distance = curr_symbol->value - code_image[curr_code_id].address;
                    if (distance < IMMEDIATE_MIN || distance > IMMEDIATE_MAX) {
                        err_report(curr_line, ERR_CODE_16);
                    }
                    else if ((code_image[curr_code_id].machine_code & IMMEDIATE_FIELD_MASK) == '?') {
                        curr_encode_word = encode_word(code_image[curr_code_id].machine_code, distance, IMMEDIATE_FIELD_MASK);
                        update_code_machine_code(curr_code_id, curr_encode_word);
                    }
                }
            }
        }
        curr_code_id++;
    }

    fclose(origin_file);
    
    if (err_found()){
        return FALSE;
    }

    return TRUE;
}