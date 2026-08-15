#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <ctype.h>

#include "../Headers/symbol_table.h"
#include "../Headers/memory_image.h"
#include "../Headers/first_pass.h"
#include "../Headers/instructions.h"
#include "../Headers/parser.h"
#include "../Headers/errors.h"

static void add_directive_data(int count , long values[], char name[], int *dc){

    int i, size ;
    size = 0;

    if (strcmp(name,".db") == 0){
        size = DB_BYTE_SIZE;
    }

    else if (strcmp(name,".dh") == 0){   
        size = DH_BYTE_SIZE;     
    }

    else if (strcmp(name,".dw") == 0){
        size = DW_BYTE_SIZE;  
    }

    for (i = 0; i < count; i++){
        add_data_line(values[i], size);
        (*dc) = (*dc) + (size);     
    }
}

static unsigned int parse_register(const char op[], Line line){

    long value;

    /*convert the char to be a number*/
    if (op[0] == '$'){
        value = atol(op+1);
    }
    else{
        value = atol(op);
    }

    /*if it is a valide register its need to be up to 31 from 0*/
    if (value >= FIRST_REG && value <= LAST_REG ){
        return (unsigned int)value;
    }
    err_report(line,ERR_CODE_14);
    return 0;
}

static unsigned long to_binery(Parsed_line curr_line, char ops[MAX_OPERANDS][MAX_OPERAND_LEN], Line line){

    char *endptr;
    union machine_code_R r_machine_code;
    union machine_code_I i_machine_code;
    union machine_code_J j_machine_code;

    if(curr_line.instruction->type == R_C_TYPE || curr_line.instruction->type == R_A_TYPE ){   
        r_machine_code.all_in_one = 0;
        r_machine_code.Fields.opcode = curr_line.instruction->opcode;
        r_machine_code.Fields.rs = parse_register(ops[0],line);
        r_machine_code.Fields.rt = (curr_line.instruction->type == R_C_TYPE) ? 0 : parse_register(ops[1],line);
        r_machine_code.Fields.rd = (curr_line.instruction->type == R_C_TYPE) ? parse_register(ops[1] ,line): parse_register(ops[2], line);        
        r_machine_code.Fields.funct  = curr_line.instruction->funct;
        r_machine_code.Fields.unuse  = 0;
        
        return r_machine_code.all_in_one;
    }

    if(curr_line.instruction->type == I_A_TYPE || curr_line.instruction->type == I_B_TYPE || curr_line.instruction->type == I_M_TYPE){   
        i_machine_code.all_in_one = 0;
        i_machine_code.Fields.opcode = curr_line.instruction->opcode;
        i_machine_code.Fields.rs = parse_register(ops[0],line);

        if (curr_line.instruction->type == I_A_TYPE || curr_line.instruction->type == I_M_TYPE){
            i_machine_code.Fields.rt = parse_register(ops[2], line);
            i_machine_code.Fields.immed = (unsigned int) strtol(ops[1], &endptr, 10);
        }  

        if (curr_line.instruction->type == I_B_TYPE){
            i_machine_code.Fields.rt = parse_register(ops[1], line);   
            i_machine_code.Fields.immed = 63;
        
        }  

        return i_machine_code.all_in_one;
    }

    if(curr_line.instruction->type == J_TYPE ){
        j_machine_code.all_in_one = 0;
        j_machine_code.Fields.opcode = curr_line.instruction->opcode;

        if (strcmp(curr_line.name ,"hlt") == 0){
            j_machine_code.Fields.reg = 0;
            j_machine_code.Fields.address = 0;
        }

        else{
            j_machine_code.Fields.reg = (ops[0][0] == '$') ? 1 : 0;
            j_machine_code.Fields.address = (j_machine_code.Fields.reg == 1 ) ?  parse_register(ops[0],line) : QUESTION_MARK ;
        }
        
        return j_machine_code.all_in_one;
    }
    return 0;
}

Bool first_pass(FILE *am, const char *file_name, int *icf_curr_line, int *dcf_curr_line)
{
    Line line ;
    Parsed_line curr_line;
    Bool is_label_def;
    char copy_line[MAX_LINE_LEN];
    char full_directive_str[MAX_LINE_LEN];
    char full_instruction_str[MAX_LINE_LEN];
    char ops[MAX_OPERANDS][MAX_OPERAND_LEN];
    unsigned long all_in_one;
    int i,line_num, ic, dc, count;
    long values[MAX_LINE_LEN];   


    line_num = 1;
    ic = START_ADDRESS;   /* 100 */
    dc = 0;
    
    line.file_name = file_name;
    line.line_num = line_num ;

    while (fgets(copy_line, MAX_LINE_LEN, am) != NULL){   
        count = 0;
        
        for (i=0; i < MAX_OPERANDS; i++){
            ops[i][0] = '\0';
        }
        
        is_label_def = FALSE;
        line.data = copy_line;

        if (!split_line(line, &curr_line)){
            line.line_num++;  
            continue; 
        }

        if (curr_line.kind == LINE_EMPTY || curr_line.kind ==  LINE_COMMENT || curr_line.kind ==  LINE_INVALID){
            line.line_num++; 
            continue;
        }
        
        if (!is_valid_rest_split(line, &curr_line, ops, values, &count)){
            line.line_num++;
            continue;
        }

        if (curr_line.label[0] != '\0'){
            is_label_def = TRUE;
        }
        
        if (curr_line.kind == LINE_DIRECTIVE && strcmp(curr_line.name,".entry") != 0 && strcmp(curr_line.name,".extern") != 0 ){   
            sprintf(full_directive_str, "%s %s %s", curr_line.label, curr_line.name, curr_line.rest);

            if (is_label_def){          
                if (symbol_search(curr_line.label) != NULL){
                    err_report(line , ERR_CODE_29);
                    line.line_num++;
                    continue;
                }
                symbol_add(curr_line.label, S_DATA, dc);
            }

            if (strcmp(curr_line.name,".asciz") == 0){                      
                count = 1;
                while (curr_line.rest[count] != '"'){            
                    add_data_line(curr_line.rest[count], 1);
                    count++ ;
                    dc += 1; 
                }
                add_data_line('\0', 1);
                dc += 1;
                line.line_num++;
                continue;
            }
            else{   
                add_directive_data(count, values, curr_line.name, &dc);
                line.line_num++;
                continue;
            }
        }

        if(strcmp(curr_line.name, ".entry") == 0){
            line.line_num++;
            continue;
        }
        else if (strcmp(curr_line.name, ".extern") == 0){
            symbol_add(curr_line.rest, S_EXTERNAL, 0);
            line.line_num++;
            continue;   
        }
        
        if (curr_line.kind ==  LINE_INSTRUCTION){   
            if (is_label_def){   
                if (symbol_search(curr_line.label) != NULL){
                    err_report(line , ERR_CODE_29);
                    line.line_num++;
                    continue;
                }
                symbol_add(curr_line.label, S_CODE, ic);
            }
            
            all_in_one = to_binery(curr_line,ops,line);

            sprintf(full_instruction_str, "%s %s %s", curr_line.label, curr_line.name, curr_line.rest);
            add_code_line(full_instruction_str, all_in_one);
        
            line.line_num++;
            ic += 4;
        }
    }

    if (err_found()){
        return FALSE;
    }
    
    *icf_curr_line = ic;
    *dcf_curr_line = dc;

    symbol_table_shift_data(*icf_curr_line);

    return TRUE;
} 