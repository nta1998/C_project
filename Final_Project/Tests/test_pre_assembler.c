#include <stdio.h>
#include "pre_assembler.h"
#include "errors.h"

int main(void){
    printf("--- valid1 ---\n");
    printf(pre_assembler("valid1.as", "valid1.am") ? "SUCCESS\n" : "FAILED (unexpected!)\n");

    printf("--- valid2 ---\n");
    printf(pre_assembler("valid2.as", "valid2.am") ? "SUCCESS\n" : "FAILED (unexpected!)\n");

    printf("--- invalid1 ---\n");
    printf(pre_assembler("invalid1.as", "invalid1.am") ? "SUCCESS (unexpected!)\n" : "FAILED (as expected)\n");

    return 0;
}