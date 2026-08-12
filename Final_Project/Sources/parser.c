#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../Headers/instructions.h"
#include "../Headers/globals.h"
#include "../Headers/parser.h"
#include "../Headers/errors.h"


Bool line_split(const char *raw, Parsed_line *out, Line line){
  
    int i = 0;
    
    /*out clean*/
    out->label[0] = '\0';
    out->name[0] = '\0';
    out->rest[0] = '\0';

    /*skip all the space*/
    skip_white_space(raw,&i);
    
    /*if the line is empty*/
    if(raw[i] == '\0'){
        out->kind = LINE_EMPTY;
        return TRUE;
    }

    /*if the line is comment*/
    if (raw[i] == ';')
    {   
        out->kind = LINE_COMMENT;
        return TRUE;
    }
    /*split the first word*/
    splid_label(raw,out,&i,line);
    /*split the second word if the first word is a label*/
    if (out->name[0] == '\0'){splid_name(raw,out,&i,line);}
    /*split the rest of the line*/
    splid_rest(raw,out,&i);  
    /*filter the line kind*/

    kind_test(out,line);
    if (out->kind != LINE_INVALID)
    {
        return TRUE;  
    }
    return FALSE; 
}
/* if it is a valid label its need to end in ':' char and start whit a leteer */
Bool is_valid_label(char *s,Line line){

    int label_len = strlen(s);

    if (label_len < MAX_LABEL_LEN)
    {
        if(isalpha(s[0]))
        {
            if (s[label_len-1] == ':')
            {
                if (s[label_len-2] != ' ')
                {
                    if (instruction_search(s) == NULL)
                    {   
                        s[label_len-1] = '\0';
                        return TRUE;
                    }
                    err_report(line,ERR_CODE_26);
                    return FALSE;    
                }
                err_report(line,ERR_CODE_28);
                return FALSE; 
            }
            err_report(line,ERR_CODE_27);
            return FALSE;  
        }
        err_report(line,ERR_CODE_23);
        return FALSE;
    }
    err_report(line,ERR_CODE_25);
    return FALSE;
}
Bool is_directive_word(const char *s)
{
    const char *reserved_words[] = {".dh", ".dw", ".db", ".asciz", ".entry", ".extern"};
    int i ;
    /*is it the first char id a dot*/
    if (s[0] != '.' )
    {
        return FALSE;
    }
    /*if the first char is a dot so it need to be one of the directive words */
    else
    {
        for (i=0 ; i < 6; i++)
        {
            if(strcmp(s,reserved_words[i]) == 0)
            {
                return TRUE;
            } 
        }    
    }   
    return FALSE;
}
int operands_split2(char *rest, char words[][81],Line line){

    int comma_count = 0; 
    int word_idx = 0;    
    int char_idx = 0;    
    int i = 0;
    char buffer[81];
    buffer[0] = '\0';
    if (rest[0] == '\0') 
    {
        words[0][0] = '\0';
        return 0;
    }
    /* runing on the operands part and split it by the char ',' */
    while (rest[i] != '\0') 
    {   
        /*if the char is a ','*/
        if (rest[i] == ',') 
        {
            buffer[char_idx] = '\0';
            strcpy(words[word_idx],buffer);
            i++;
            comma_count++;
            word_idx++;
            if (word_idx >= 5 || char_idx == 0) break;
            char_idx = 0; 
        } 
        /*if the char is not a ',' adding it to the buffer */
        else {
            if (char_idx < MAX_LINE_LEN - 1 && rest[i] != '$') {
                buffer[char_idx] = rest[i];
                char_idx++;
            }
            i++;
        }
    }
    /*marck the end of the last info and copy it to the word array*/
    buffer[char_idx] = '\0';
    strcpy(words[word_idx],buffer);
    /*one comma it need to between to words if is no its no valid line*/
    if (comma_count >= word_idx+1)
    {   
        err_report(line,ERR_CODE_17);
        err_report(line,ERR_CODE_18);
        return -1;
    }
    return word_idx+1;
}
/*skip all the chars if is a space */
void skip_white_space(const char *raw, int *i){
    while (isspace(raw[*i])) {(*i)++;}
}
/*splits the first word to a label slot in to Parsed_line varyebol*/
void splid_label(const char *raw, Parsed_line *out ,int *i,Line line){
    
    char buffer[MAX_LABEL_LEN+1];
    buffer[0] = '\0';
    int j = 0;
    /*keep going untile the first spase and chack if the len is less from the max len*/
    while (!isspace(raw[*i]) && j < MAX_LABEL_LEN)
    {   
        buffer[j] = raw[*i];
        j++;
        (*i)++;
    }
    /*ending the label word*/
    buffer[j]= '\0';
    if (instruction_search(buffer) != NULL || is_directive_word(buffer))
    {
        strcpy(out->name,buffer);
        out->instruction = instruction_search(out->name);
        out->kind = LINE_INSTRUCTION;
    }
    else if(is_valid_label(buffer,line)){strcpy(out->label,buffer);}
    else
    {
        out->kind = LINE_INVALID;
    }
    skip_white_space(raw,i);
}
/*splits the seccond word to a commend name slot in to Parsed_line varyebol*/
void splid_name(const char *raw, Parsed_line *out ,int *i,Line line){
    
    char buffer[MAX_COMMEND_NAME_LEN];
    buffer[0] = '\0';
    int j = 0;
    /*keep going untile the first spase*/
    while (!isspace(raw[*i]))
    {
        buffer[j] = raw[*i];
        j++;
        (*i)++;
    }
    /*ending the commend name word*/
    buffer[j] = '\0';
    if (is_directive_word(buffer))
    {
        strcpy(out->name,buffer);
        out->kind = LINE_DIRECTIVE;   
    }
    else if (instruction_search(buffer) != NULL){
        strcpy(out->name,buffer);
        out->instruction = instruction_search(out->name);
        out->kind = LINE_INSTRUCTION;   
    }
    else
    {
        err_report(line,ERR_CODE_22);
    }
    skip_white_space(raw,i);
}
/*splits the rest to a rest slot in to Parsed_line varyebol*/
void splid_rest(const char *raw, Parsed_line *out ,int *i){

    int j = 0;
    /*keep going until the end of the line or an ';' char*/
    while (raw[*i] != '\0' && raw[*i] != ';')
    {    
        if(!isspace(raw[*i]))
        {
            out->rest[j] = raw[*i];
            j++;
        }
            (*i)++;
    }
    /*ending the rest word*/
    out->rest[j] = '\0';
}
/*chacks the out kind in the end if faild the defulte is invalide line*/
void kind_test(Parsed_line *out,Line line){
    char op[10][81];
    if (out->rest[0] != '\0')
    {
        if (is_directive_word(out->name) && operands_split(out,op,line) != -1)
        {
            out->kind = LINE_DIRECTIVE;
        }
        else if (instruction_search(out->name) != NULL && operands_split(out,op,line) != -1)
        {
            out->kind = LINE_INSTRUCTION;
        }
    }
    else if (strcmp(out->name,"hlt") == 0)
    {
        out->kind = LINE_INSTRUCTION;
    }
}
int operands_split(Parsed_line *out, char words[][81],Line line){

    int i;
    int comma_count;

    if (out->kind == LINE_INSTRUCTION)
    {
        if (out->instruction->type == R_A_TYPE)
        {   
            comma_count = 0 ;
            char dest[50];
            for(i = 0; i < 3 && comma_count <= 2 ; i++) 
            {   
                char *ptr = strchr(out->rest, ',');
                if (ptr != NULL)
                {
                    size_t length = ptr - out->rest;
                    strncpy(dest, out->rest, length);
                    out->rest[length] = ' '; 
                    comma_count++;
                }
                else
                {
                    err_report(line,ERR_CODE_10);
                }
             
            }
            i++;
            if(comma_count > 2 && i = 3){err_report(line,ERR_CODE_10);}
            if (i <= comma_count)
            {
                err_report(line,ERR_CODE_18);
            }
        }
        else if (out->instruction->type == R_C_TYPE || out->instruction->type == I_A_TYPE || out->instruction->type == I_B_TYPE ||out->instruction->type == I_M_TYPE)
        {   
            comma_count = 0 ;
            char dest[50];
            for(i = 0; i < 1 && comma_count == i; i++) 
            {   
                char *ptr = strchr(out->rest, ',');
                if (ptr != NULL)
                {
                    size_t length = ptr - out->rest;
                    strncpy(dest, out->rest, length);
                    out->rest[length] = ' '; 
                    comma_count++;
                }
                else
                {
                    err_report(line,ERR_CODE_10);
                }
               
            }
            i++;
            if (i <= comma_count)
            {
                err_report(line,ERR_CODE_18);
            }
        }
        else if (out->instruction->type == J_TYPE)
        {
            comma_count = 0 ;
            char dest[50];
            char *ptr = strchr(out->rest, ',');
            if (ptr != NULL)
            {
                size_t length = ptr - out->rest;
                strncpy(dest, out->rest, length);
                out->rest[length] = ' '; 
            }
            else
            {
                err_report(line,ERR_CODE_10);
            }
            comma_count++;
        }
    }
    else if (out->kind == LINE_DIRECTIVE)
    {
        if (strcmp(out->name,".dh" || "dw" || "db") == 0)
        {   
            comma_count = 0;
            i = 0;
            char dest[50];
            while(dest[0] != '\0') 
            {   
                char *ptr = strchr(out->rest, ',');
                if (ptr != NULL)
                {
                    size_t length = ptr - out->rest;
                    strncpy(dest, out->rest, length);
                    out->rest[length] = ' '; 
                    comma_count++;
                    i++;
                }
                else
                {
                    err_report(line,ERR_CODE_17);
                }
             
            }
            i++;
            if (i <= comma_count)
            {
                err_report(line,ERR_CODE_18);
            }
        }
    }
    
    return i;
}