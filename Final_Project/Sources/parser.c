#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Headers/instructions.h"
#include "../Headers/globals.h"
#include "../Headers/parser.h"

Bool line_split(const char *raw, Parsed_line *out, const char *file, int ln){
    
    int i = 0;
    int j = 0;

    out->label[0] = '\0';
    out->name[0] = '\0';
    out->rest[0] = '\0';
    out->error = 0;


    while (raw[i] == ' '||raw[i] == '\t'||raw[i] == '\n'){i++;}
    
    if(raw[i] == '\0'){
        out->type = LINE_EMPTY;
        return TRUE;
    }
    if (raw[i] == ';')
    {   
        out->type = LINE_COMMENT;
        return TRUE;
    }

    while (raw[i] != ' ' && raw[i] != '\0' && raw[i] != '\n' && j <= MAX_LABEL_LEN)
    {
        out->label[j] = raw[i];
        j++;
        i++;
        continue;
    }
    out->label[j] = '\0';
    j = 0;
    
    if (!is_valid_label(out->label)){
        
        if (is_reserved_word(out->label) || is_directive_word(out->label)){
            while(out->label[j] != '\0')
            {     
                out->name[j] = out->label[j];
                out->label[j] = '\0';
                j++;
            }
            out->name[j] = '\0';
        }
        else
        {
            /* error */
            out->type = LINE_INVALID;
            return FALSE;
        }    
    }
        
    while (raw[i] == ' '){i++;}

    j = 0;
    if (out->label[0] != '\0')
    {
        while (raw[i] != ' ' && raw[i] != '\0')
        {
            if(raw[i] != '\n' && raw[i] != ' ')
            {
                out->name[j] = raw[i];
                j++;
            }
            i++;
            
        }
        out->name[j] = '\0';
    }

    while (raw[i] == ' '){i++;}

    j = 0;
    
    while (raw[i] != '\0')
    {    
        if(raw[i] != '\n' && raw[i] != ' ')
        {
            out->rest[j] = raw[i];
            j++;
        }
        i++;
       
    }
    out->rest[j] = '\0';
    char *ops[4];
    if (operands_split(out->rest,ops,3,file,ln) >= 0)
    {
        if (out->label[0] == '.' && is_directive_word(out->name)){
            out->type = LINE_DIRECTIVE;
        }
        else if (is_valid_label(out->label)){
            if (out->name[0] == '.' && is_directive_word(out->name))
            {
            out->type = LINE_DIRECTIVE;
            return TRUE;
            }
            else 
            {
                if(is_reserved_word(out->name))
                {
                    out->type = LINE_INSTRUCTION;
                    return TRUE;
                }
                out->type = LINE_INVALID;
                return FALSE;
            }
        }
        return TRUE; 
    }
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
Bool parse_number(const char *s, long *out){

    int i = 0;
    /*if the char heve a sing char like '-' or '+' in the start its approved*/
    if (s[0] == '+' || s[0] == '-')
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

    /*all the string is a valid number */
    *out = atol(s);    
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
int parse_register(const char *s){
    int i = 1;
    long value;
    if (s[0] != '$')
    {
        return FALSE;
    };
    
    if (s[i] == '\0')
    {
        return FALSE;
    };

    while (s[i] != '\0')
    {
        if (s[i] < '0' || s[i] > '9')
        {
            return FALSE;
        }
        i++;
    }
    value = atol(s+1);
    if (value >= 0 && value <= 31)
    {
        return value;
    };
    return FALSE;
}
int operands_split(char *rest, char *ops[], int max_ops, const char *file, int ln){
    
    int i = 0;
    int j = 0;
    Bool invalid_stract = FALSE;
    Bool cumo_nedded = FALSE;
    while (rest[i] == ' '){i++;}
    if (rest[i] == ','){return -1;}
    while (rest[i] != '\0')
    {
        if (rest[i] == ' ')
        {
            i++;
            continue;
        }
        else if (rest[i] == ',')
        {
            if (invalid_stract)
            {
                return -1;
            }
            cumo_nedded = FALSE;
            invalid_stract = TRUE;
            i++;
            j++;
            continue;
        }
        else
        {   
            if (cumo_nedded)
            {
                return -1;
            }
            invalid_stract = FALSE;
            ops[j] = ops[j] + rest[i];
            i++;
            continue;
        }
        return -1;
    }
    if (invalid_stract)
    {
        return -1;
    }
    return j;
 
}