#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
    if (is_valid_label(out))
    {   
        skip_white_space(raw,&i);
        splid_name(raw,out,&i);
        skip_white_space(raw,&i);
        splid_rest(raw,out,&i);
        out->instruction = instruction_search(out->name);
        kind_test(out,line);
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
            kind_test(out,line);
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
    
    kind_test(out,line);
    return FALSE;
}
/* if it is a valid label its need to end in the char ':' and be the unice */
Bool is_valid_label(Parsed_line *s){
    int i = strlen(s->label);
    if((s->label[0] >= 'A' && s->label[0] <= 'Z') || (s->label[0] >= 'a' && s->label[0] <= 'z'))
    {
        if (s->label[i-1] == ':' && !it_is_reserved_word(s->label) && i <= 32)
        {   
            s->label[i-1] = '\0';
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}
Bool it_is_reserved_word(const char *s){
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
/*dell ?*/
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
        for (i=0; i < 6; i++)
        {
            if(strcmp(s,reserved_words[i]) == 0)
            {
                return TRUE;
            } 
        }    
    }   
    return FALSE;
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

int operands_split(char *rest, char words[][81],Line line){
    
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
/*if the char is a space */
Bool it_is_space(char c) {
    return (c == WHITE_SPACE || c == '\n' || c == '\t' || c == '\r' || c == ' ');
}
/*skip all the chars if is a space */
void skip_white_space(const char *raw, int *i){
    while (it_is_space(raw[*i])) {(*i)++;}
}
/*splits the first word to a label slot in to Parsed_line varyebol*/
void splid_label(const char *raw, Parsed_line *out ,int *i){
    
    int j = 0;
    /*keep going untile the first spase and chack if the len is less from the max len*/
    while (!it_is_space(raw[*i]) && j < MAX_LABEL_LEN)
    {
        out->label[j] = raw[*i];
        j++;
        (*i)++;
    }
    /*ending the label word*/
    out->label[j] = '\0';
}
/*splits the seccond word to a commend name slot in to Parsed_line varyebol*/
void splid_name(const char *raw, Parsed_line *out ,int *i){
    
    int j = 0;
    /*keep going untile the first spase*/
    while (!it_is_space(raw[*i]))
    {
        out->name[j] = raw[*i];
        j++;
        (*i)++;
    }
    /*ending the commend name word*/
    out->name[j] = '\0';
   
}
/*splits the rest to a rest slot in to Parsed_line varyebol*/
void splid_rest(const char *raw, Parsed_line *out ,int *i){

    int j = 0;
    /*keep going until the end of the line or an ';' char*/
    while (raw[*i] != '\0' && raw[*i] != ';')
    {    
        if(raw[*i] != '\n' && raw[*i] != ' ' && raw[*i] != '"' && raw[*i] != '\r')
        {
            out->rest[j] = raw[*i];
            j++;
        }
            (*i)++;
    }
    /*ending the rest word*/
    out->rest[j] = '\0';
}
/*move the labe info in to the name if the label is not valid but it a valid commend */
void move_form_label_to_name(Parsed_line *out){

    int j = 0 ;
    /*keep going untile the end of the label chars*/
    while(out->label[j] != '\0')
    {     
        out->name[j] = out->label[j];
        out->label[j] = '\0';
        j++;
    }
    /*ending the commend name word*/
    out->name[j] = '\0';
}
/*chacks the out kind in the end if faild the defulte is invalide line*/
void kind_test(Parsed_line *out,Line line){
    char op[10][81];
    if (out->rest[0] != '\0')
    {
        if (is_directive_word(out->name) && operands_split(out->rest,op,line) != -1)
        {
            out->kind = LINE_DIRECTIVE;
        }
        else if (it_is_reserved_word(out->name) && operands_split(out->rest,op,line) != -1)
        {
            out->kind = LINE_INSTRUCTION;
        }
    }
    else if (strcmp(out->name,"hlt") == 0)
    {
        out->kind = LINE_INSTRUCTION;
    }
}