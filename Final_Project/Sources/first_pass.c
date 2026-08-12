#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <ctype.h>
#include "../Headers/globals.h"
#include "../Headers/first_pass.h"
#include "../Headers/instructions.h"
#include "../Headers/parser.h"
#include "../Headers/errors.h"

Bool first_pass(FILE *am, const char *filename, int *icf_out, int *dcf_out)
{
    char line[MAX_LINE_LEN];
    int line_num = 1;
    Parsed_line pl;
    char full_directive_str[MAX_LINE_LEN];
    char full_instruction_str[MAX_LINE_LEN];
    unsigned long all_in_one;
    char ops[10][81];

    /*step 1*/
    int ic = START_ADDRESS;   /* 100 */
    int dc = 0;
    
    Line e ;
    e.file_name = filename;
    e.line_num = line_num ;
    /* step 2 */
    while (fgets(line, sizeof(line), am) != NULL) 
    {   
        
        int has_label = 0;
        e.data = line;
        if (strlen(line) > MAX_LINE_LEN)
        {
            err_report(e,ERR_CODE_9);
        }

        if (!line_split(line, &pl, e)) 
        {   e.line_num++;  
            continue; }

        /*step 3*/
        if (pl.kind == LINE_EMPTY || pl.kind ==  LINE_COMMENT || pl.kind == LINE_INVALID){e.line_num++; continue;}
        
        /*step 4*/
        if (pl.label[0] != '\0'){/*step 5*/ has_label = 1;}
        
        /*step 6*/
        if (pl.kind == LINE_DIRECTIVE && strcmp(pl.name,".entry") != 0 && strcmp(pl.name,".extern") != 0 )
        {   
            sprintf(full_directive_str, "%s %s %s", pl.label,pl.name,pl.rest);

            /*step 7*/
            if (has_label)
            {          
                if (symbol_search(pl.label) != NULL)
                {
                    err_report(e , ERR_CODE_28);
                }
                /*צריך שגיאה של תוית לא תקינה*/
                symbol_add(pl.label,S_DATA,dc);
            }

            /*step 8*/
            if (strcmp(pl.name,".asciz") == 0)   
            {                      
                int cou = 0;
                while (pl.rest[cou] != '\0')
                {            
                    add_data_line(pl.rest[cou],1);
                    cou++ ;
                    dc = dc + 1; 
                }
                add_data_line('\0',1);
                dc = dc + 1;
                e.line_num++;
                continue;
            }
            else
            {   
               int amunt = operands_split(pl.rest,ops,e);
                add_directive_data(amunt,ops,pl.name,&dc);
                e.line_num++;
                continue;
            }
        }
        /*step 9*/
        if(strcmp(pl.name,".entry") == 0)
        {
            /*step 10*/
            e.line_num++;
            continue;
        }
        else if (strcmp(pl.name,".extern") == 0)
        {
            /*step 11*/
            symbol_add(pl.rest,S_EXTERNAL,0);
            e.line_num++;
            continue;   
        }
        
        if (pl.kind ==  LINE_INSTRUCTION)
        {   
            /*step 12*/
            if (has_label)
            {   
                if (symbol_search(pl.label) != NULL)
                {
                    err_report(e , ERR_CODE_28);
                }
                symbol_add(pl.label,S_CODE,ic);
            }

            /* step 13*/
            if (pl.instruction == NULL) 
            {
                Line e;
                e.file_name = filename; e.line_num = line_num; e.data = line;
                err_report(e, ERR_CODE_21);   /* "Instruction does not exist." */
                e.line_num++;
                continue;
            }
            
            /*step 14*/
            operands_split(pl.rest,ops,e);
            
            /*step 15*/
            all_in_one = to_binery(pl,ops,e);

            sprintf(full_instruction_str, "%s %s %s", pl.label, pl.name,pl.rest);
            add_code_line(full_instruction_str,all_in_one);
        
            /*step 16*/
            e.line_num++;
            ic = ic + 4;
        }
    }

    /*step 17*/
    if (err_found()){return FALSE;}
    
    /*step 18*/
    *icf_out = ic;
    *dcf_out = dc;

    /*step 19*/
    symbol_table_shift_data(*icf_out);
    /*step 20*/


    /*step 21*/
    return TRUE;
}    

void add_directive_data(int pram_num , char ops[][81],char name[],int *dc)
{
    unsigned int test;
    int size ;
    int i ;
    size = 0;
    if (strcmp(name,".db") == 0) 
    {
        size = 1;
    }
    else if (strcmp(name,".dh") == 0)
    {   
        size = 2;     
    }
    else if (strcmp(name,".dw") == 0)
    {
        size = 4;  
    }
    for (i = 0; i < pram_num; i++)
    {
        test = atol(ops[i]);
        add_data_line(test,size);
        (*dc) = (*dc) + (size);     
    }
}
unsigned int parse_register(const char s[],Line line){

    unsigned int value;
    /*convert the char to be a number*/
    value = atol(s);
    /*if it is a valide register its need to be up to 31 from 0*/
    if (value >= 0 && value <= 31)
    {
        return value;
    }
    err_report(line,ERR_CODE_14);
    return FALSE;
}
unsigned long to_binery(Parsed_line pl, char ops[][81],Line line){

    union machine_code_R r_machine_code;
    union machine_code_I i_machine_code;
    union machine_code_J j_machine_code;

    if(pl.instruction->type == R_C_TYPE || pl.instruction->type == R_A_TYPE )
    {   

        r_machine_code.all_in_one = 0;
        r_machine_code.fields.opcode = pl.instruction->opcode;
        r_machine_code.fields.rs = parse_register(ops[0],line);
        r_machine_code.fields.rt = (pl.instruction->type == R_C_TYPE) ? 0 : parse_register(ops[1],line);
        r_machine_code.fields.rd = (pl.instruction->type == R_C_TYPE) ? parse_register(ops[1] ,line): parse_register(ops[2], line);        
        r_machine_code.fields.funct  = pl.instruction->funct;
        r_machine_code.fields.unuse  = 0;
        
        return r_machine_code.all_in_one;
    }
    if(pl.instruction->type == I_A_TYPE || pl.instruction->type == I_B_TYPE || pl.instruction->type == I_M_TYPE)
    {   
        char *endptr;
        i_machine_code.all_in_one = 0;
        i_machine_code.fields.opcode = pl.instruction->opcode;
        i_machine_code.fields.rs = parse_register(ops[0],line);
        if (pl.instruction->type == I_A_TYPE || pl.instruction->type == I_M_TYPE)
        {
            i_machine_code.fields.rt = parse_register(ops[2], line);
            i_machine_code.fields.immed = (unsigned int) strtol(ops[1], &endptr, 10);
        }   
        if (pl.instruction->type == I_B_TYPE)
        {
            i_machine_code.fields.rt = parse_register(ops[1], line);   
            i_machine_code.fields.immed = 63;
        
        }  
        return i_machine_code.all_in_one;
    }
    if(pl.instruction->type == J_TYPE )
    {
        j_machine_code.all_in_one = 0;
        j_machine_code.fields.opcode = pl.instruction->opcode;
        if (strcmp(pl.name ,"hlt") == 0)
        {
            j_machine_code.fields.reg = 0;
            j_machine_code.fields.address = 0;
        }
        else
        {
            j_machine_code.fields.reg = (isdigit(ops[0][0]) == FALSE) ? 0 : 1;
            j_machine_code.fields.address = (j_machine_code.fields.reg == 0 ) ? 63 : parse_register(ops[0],line);
        }
        
        return j_machine_code.all_in_one;
    }
    return FALSE;
}
