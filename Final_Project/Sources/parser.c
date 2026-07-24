#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Headers/instructions.h"
#include "../Headers/globals.h"
#include "../Headers/parser.h"

Bool line_split(const char *raw, ParsedLine *out, const char *file, int ln){

    int i = 0;
    int j = 0;
    
    Bool label_flag = FALSE;
    Bool directive_flag = FALSE;

    /*skiping all the speas in the start*/
    while (raw[i] == ' '){i++;}

    /*if the loop end and the flag is not on so it can be only empty line */
    while (raw[i] != ' ' && raw[i] != '\0'){

        /*the line is a commennt if its start in the char ';' and all the flags are off */
        if (raw[i] == ';' && !label_flag && !directive_flag)
        {   
            out->kind = LINE_COMMENT;
            return TRUE;
        }

        /*the line can be a directive or invalid */
        if (raw[i] == '.' || directive_flag)
        {
            directive_flag = TRUE;
            out->name[j] = raw[i];
            j++;
            i++;
            continue;
        }

        /*the line can be directive or instuction or invalid*/
        if (raw[i] == ':' || (raw[i] >= 'A' && raw[i] <= 'Z') || (raw[i] >= 'a' && raw[i] <= 'z') || label_flag)
        { 
            label_flag = TRUE;
            out->label[j] = raw[i];
            j++;
            i++;
            continue;
        }

        /* if the start of a line is not starting in a leter or '.' or ';' is invalid line */
        else{
            out->kind = LINE_INVALID;
            return FALSE;
        }
   
        
    };

    /* testing the line is ended whit all the flag off so it can be only empty line*/
    if (!label_flag && !directive_flag && raw[i] == '\0')
    {
        out->kind = LINE_EMPTY;
        return TRUE;   
    };

    /* if the label flag is on so the kind of the line can be invalid or directive or instuction */
    if (label_flag)
    {
        /*the char in raw[i] can be only ' ' the while loop in line 17 stop so we need to move the i counter by 1 */
        i++;
        /*moveing to the farst char*/
        while (raw[i] == ' '){i++;}
        /*if the label is valid we need to add the rest of the chars in the raw too the name */
        if (is_valid_label(out->label)){
            /*adding too name until we get to the end or the farst ' ' char */
            j = 0;
            while (raw[i] != ' ' && raw[i] != '\0'){
                out->name[j] = raw[i];
                j++;
                i++;
                continue;                
            }

        } 

        /* if the label is not valid and the directive flag is off its can be only instruction line or invalid */
        else if (!directive_flag)
        {   
            /* if the word is reserved and we know the line canot be directive so the line is most likely too be instruction if not is invalid*/
            if (is_reserved_word(out->label))
            {
                out->kind = LINE_INSTRUCTION;
                strcpy(out->name, out->label);
                out->label[0] = '\0';
                j = 0;
                while (raw[i] != '\0'){
                    out->rest[j] = raw[i];
                    j++;
                    i++;
                    continue; 
                }
                return TRUE;
            }
            
            out->kind = LINE_INVALID;
            return FALSE;
        }
        
    }
    /*if the directive flag is on we can get only directive or invalid*/
    else if (directive_flag)
    {
        /*the char in raw[i] can be only ' ' the while loop in line 17 stop so we need to move the i counter by 1 */
        i++;
        /*moveing to the farst char*/
        while (raw[i] == ' '){i++;}
        /* if the word is reserved and we know the line is not instruction so the line is most likely too be directive if not is invalid*/
        if (is_reserved_word(out->name))
        {   
            out->kind = LINE_DIRECTIVE;
            j = 0;
            while (raw[i] != '\0')
            {
                out->rest[j] = raw[i];
                j++;
                i++;
                continue; 
            }
            return TRUE;
        }
        
    }
    out->kind = LINE_INVALID;
    return FALSE;
}

/* if it is a valid label its need to end in the char ':' and be the unice */
Bool is_valid_label(const char *s){
    int i = 0;
    /* check if the label ends in char ':' */
    while ( *(s+i) != ' ' && *(s+i) != '\0') 
    {
        i++;
    } 
    if (*(s+i-1) == ':' && !is_reserved_word(s) && i <= 32)
    {
        return TRUE;
    }

    return FALSE;
}

Bool is_reserved_word(const char *s){
    /*if the word is a reserve word its can be a insttruction or a saved label*/

    /*if the word is a insttruction*/
    if (inst_find(s) != NULL)
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
