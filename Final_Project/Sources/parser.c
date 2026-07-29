#include <stdio.h>
#include <string.h>
#include "../Headers/globals.h"
#include "../Headers/parser.h"

Bool line_split(const char *raw, ParsedLine *out, const char *file, int ln){

    int i = 0;

    /*if the loop end so is can be empty or invalid*/
    while (raw[i] != ' ' && raw[i] != '\0'){
        /*kind*/
        /*the line is a commennt*/
        if (raw[i] == ';' )
        {   
            out.kind == LineKind.LINE_COMMENT
            return TRUE
        };

        /*kind and name*/
        /*the line can be a directive*/
        else if (raw[i] == '.')
        {

            if (strncmp(raw + i,".dh ", 4) == TRUE)
            {   
                out.kind == LineKind.LINE_DIRECTIVE
                out.name == "dh";
            };
            
            if (strncmp(raw + i,".dw ", 4) == TRUE)
            {
                out.kind == LineKind.LINE_DIRECTIVE
                out.name == "dw";
            };

            if (strncmp(raw + i,".db ", 4) == TRUE)
            {
                out.kind == LineKind.LINE_DIRECTIVE
                out.name == "db";
            };

            if (strncmp(raw + i,".asciz ", 7) == TRUE)
            {   
                out.kind == LineKind.LINE_DIRECTIVE
                out.name == "asciz";
            };
            
            if (strncmp(raw + i,".entry ", 7) == TRUE)
            {   
                out.kind == LineKind.LINE_DIRECTIVE
                out.name == "entry";
            };

            if (strncmp(raw + i,".extern ", 8) == TRUE)
            {   
                out.kind == LineKind.LINE_DIRECTIVE;
                out.name == "extern";
            };
        };

        /*label*/
        else if (is_valid_label(raw[i]))
        {
            out.label == "";
        }

        /*rest*/    
        else
        {
            out.rest = out.rest + raw[i];
        };
        
        i++;

    };

    out.kind == LineKind.LINE_EMPTY;
    return TRUE;
};

Bool is_valid_label(const char *s){
    /*chaking if the farst char is a number*/
    if (s[0] < '0' || s[0] > '9')
    {   
        /*loking for the char ':' for a valid label */
        int j == 0 ;
        while (s[j] != ' ' && s[j] != '\0')
        {
            if (s[j] == ':')
            {
                return TRUE;
            };
            j++;
        };
    };
    /*the farst char is a number or the label is ended in the char ':'*/
    return FALSE
};
Bool is_reserved_word(const char *s){};
Bool parse_number(const char *s, long *out){};
int parse_register(const char *s){};
int operands_split(char *rest, char *ops[], int max_ops, const char *file, int ln){};
