#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Headers/instructions.h"
#include "../Headers/globals.h"
#include "../Headers/parser.h"


Bool line_split(const char *raw, Parsed_line *out, const char *file, int ln){
    
    int i = 0;

    /*out clean*/
    out->label[0] = '\0';
    out->name[0] = '\0';
    out->rest[0] = '\0';

    /*skip all the space*/
    skip_white_space(raw,&i);
    /*if the line is comment or empty*/
    if(raw[i] == '\0'){
        out->kind = LINE_EMPTY;
        return TRUE;
    }
    if (raw[i] == ';')
    {   
        out->kind = LINE_COMMENT;
        return TRUE;
    }
    /*split the ferst word*/
    splid_label(raw,out,&i);
    /*if the label is valid*/
    if (is_valid_label(out->label))
    {
        skip_white_space(raw,&i);
        splid_name(raw,out,&i);
        skip_white_space(raw,&i);
        splid_rest(raw,out,&i);
        out->instruction = instruction_search(out->name);
        kind_test(out);
        if (out->kind != LINE_INVALID)
        {
            return TRUE;  
        }
        return FALSE;
    }
    /*if the label is not valid*/
    else
    {   

        out->instruction = instruction_search(out->label);
        /*if the label is a instruction or directive*/
        if (out->instruction != NULL || is_directive_word(out->label)){
            move_form_label_to_name(out);
            skip_white_space(raw,&i);
            splid_rest(raw,out,&i); 
            kind_test(out);
            if (out->kind != LINE_INVALID)
            {
                return TRUE;  
            }
            return FALSE;
        }
        /*if is not soo is invalid line*/
        else
        {
            /* error */
            out->kind = LINE_INVALID;
            return FALSE;
        }
    }
    
    kind_test(out);
    return FALSE;
}
/* if it is a valid label its need to end in the char ':' and be the unice */
Bool is_valid_label(const char *s){
    int i = 0;
    char s_tmp[32];
    if((s[0] >= 'A' && s[0] <= 'Z') || (s[0] >= 'a' && s[0] <= 'z'))
    {
        /* check if the label ends in char ':' */
        while ( *(s+i) != ' ' && *(s+i) != '\0') 
        {
            s_tmp[i] = *(s+i);
            i++;
        } 
        s_tmp[i-1] = '\0';
        if (*(s+i-1) == ':' && !is_reserved_word(s_tmp) && i <= 32)
        {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}
Bool is_reserved_word(const char *s){
    /*if the word is a reserve word its can be a insttruction or a saved label*/
    
    /*if the word is a insttruction*/
    if (instruction_search(s) != NULL)
    {
        return TRUE;
    }
    /*if the word is a saved label*/
    /*else if ()
    {
        
    }*/
    /*the word is not reserved*/
    else{
        return FALSE;
    }
    
    
}
Bool parse_number(const char *s){

    int i = 0;
    /*if the char heve a sing char like '-' or '+' in the start its approved*/
    if (s[0] == '+' || s[0] == '-' || s[0] == '$')
    {
        i++;
    }
    
    /*if the the char is only the sing or empty its canot be a valid number*/
    if (s[i] == '\0')
    {
        return FALSE;
    }

    /*if the start of the char is valide we need to test the rest of the char to be only numbers */
    while (s[i] != '\0')
    {
        /*if the char is not a number the hole sreing canot be a valid number*/
        if (s[i] < '0' || s[i] > '9')
        {
            return FALSE;
        }
        i++;
    };

    return TRUE;
}
Bool is_directive_word(const char *s){
    if (s[0] != '.' )
    {
        return FALSE;
    }
    else if (s[1] == 'd'||s[1] == 'a'|| s[1] == 'e')
    {
        return TRUE;
    }   
    else 
        return FALSE;
}
unsigned int parse_register(const char s[]){

    unsigned int value;

    value = atol(s);

    if (value >= 0 && value <= 31)
    {
        return value;
    };
    return FALSE;
}
int operands_split(char *rest, char words[][10]){
    
    int comma_count = 0; 
    int word_idx = 0;    
    int char_idx = 0;    
    int i = 0;
    char buffer[10];
    buffer[0] = '\0';

    while (rest[i] != '\0') 
    {
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
        else {
            if (char_idx < MAX_LINE_LEN - 1 && rest[i] != '$') {
                buffer[char_idx] = rest[i];
                char_idx++;
            }
            i++;
        }
    }
    buffer[char_idx] = '\0';
    strcpy(words[word_idx],buffer);
    if (comma_count >= word_idx+1 || char_idx == 0)
    {
        return -1;
    }
    return word_idx+1;
}
Bool it_is_space(char c) {
    return (c == WHITE_SPACE || c == '\n' || c == '\t' || c == '\r');
}
void skip_white_space(const char *raw, int *i){
    while (it_is_space(raw[*i])) {(*i)++;}
}
void splid_label(const char *raw, Parsed_line *out ,int *i){
    
    int j = 0;
    
    while (!it_is_space(raw[*i]) && j <= MAX_LABEL_LEN)
    {
        out->label[j] = raw[*i];
        j++;
        (*i)++;
    }
    out->label[j] = '\0';
}

void splid_name(const char *raw, Parsed_line *out ,int *i){
    
    int j = 0;

    while (!it_is_space(raw[*i]))
    {
        out->name[j] = raw[*i];
        j++;
        (*i)++;
    }
    out->name[j] = '\0';
   
}
void splid_rest(const char *raw, Parsed_line *out ,int *i){

    int j = 0;

    while (!it_is_space(raw[*i]) && raw[*i] != ';')
    {    
        if(raw[*i] != '\n' && raw[*i] != ' ' && raw[*i] != '"' )
        {
            out->rest[j] = raw[*i];
            j++;
        }
            (*i)++;
    }
    out->rest[j] = '\0';
}
void move_form_label_to_name(Parsed_line *out){

    int j = 0 ;
    while(out->label[j] != '\0')
    {     
        out->name[j] = out->label[j];
        out->label[j] = '\0';
        j++;
    }
    out->name[j] = '\0';
}
void kind_test(Parsed_line *out){
    char op[10][10];
    if (out->rest[0] != '\0')
    {
        if (is_directive_word(out->name) && operands_split(out->rest,op) != -1)
        {
            out->kind = LINE_DIRECTIVE;
        }
        else if (is_reserved_word(out->name) && operands_split(out->rest,op) != -1)
        {
            out->kind = LINE_INSTRUCTION;
        }
    }
    else if (strcmp(out->name,"hlt") == 0)
    {
        out->kind = LINE_INSTRUCTION;
    }
}