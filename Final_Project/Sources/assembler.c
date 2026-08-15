/**
 * @file assembler.c
 * @brief The entry point of the assembler.
 *
 * Receives the source files as command line arguments and runs each one
 * through the full assembly process: pre-assembler, first pass and second
 * pass. The output files are written only for a file that passed all of the
 * stages with no errors.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../Headers/globals.h"
#include "../Headers/errors.h"
#include "../Headers/pre_assembler.h"
#include "../Headers/symbol_table.h"
#include "../Headers/mcro_table.h"
#include "../Headers/external_table.h"
#include "../Headers/memory_image.h"
#include "../Headers/output.h"
#include "../Headers/first_pass.h"
#include "../Headers/second_pass.h"
#include "../Headers/parser.h"


/**
 * Builds a file name with a different extension, by replacing everything
 * from the last '.' of the original name with the new extension.
 * @param orig_file: the original file name.
 * @param new_file: out-param, the buffer that receives the new file name.
 * @param new_ext: the new extension, including the leading '.'.
*/
static void change_extension(const char *orig_file, char *new_file, const char *new_ext){
    strcpy(new_file, orig_file);
    *strrchr(new_file, '.') = '\0';
    strcat(new_file,new_ext);
}

/**
 * Resets all of the global tables and the error state, so that every source
 * file is assembled from a clean state and is not affected by the file that
 * was assembled before it.
*/
static void reset_all_tables(void){
    err_reset();
    mcro_table_reset();
    symbol_table_reset();
    external_table_reset();
    memory_image_reset();
}

/**
 * The main function of the assembler. Runs every '.as' file that was given in
 * the command line through the whole assembly process, one after the other.
 * A file that contains errors does not stop the run, the errors are reported
 * and the next file is assembled.
 * @param argc: the number of command line arguments.
 * @param argv: the command line arguments, argv[1] and on are the source files.
 * @return 1 in every case.
*/
int main (int argc, char *argv[]) {
    FILE *am_fp; 
    int icf, dcf; 
    Bool first_pass_ok;
    int i, len;
    char *as_file;
    char am_file[256];
    FILE *orig_file;

    /* If no files were attached this is not an error, the program simply does not run 
    and returns the value 1, meaning everything is fine. */
    if (argc == 1){
        printf("No files found.");
        return 1;
    }

    /*If we got here, at least one file was received in the terminal together
      with the command line, the first file is at argv[1]*/
    for (i = 1; i < argc; i++){

        /*On every new file we reset all of the counters and all of the tables
          that exist from the previous run, so that the current run starts from scratch.*/
        reset_all_tables();

        /*On every run we go over a whole file, we update the current file to be
          the next one in turn and update the length of the file name*/
        as_file = argv[i];
        len = strlen(as_file);

        if (len < 3){
            printf("Invalid file name.");
            continue;
        }
        
        /*We check whether the extension of the file is valid, meaning it ends with ".as"*/
        /*If not, we print a warning message that the file does not have the right
          extension and continue to the next file without checking it at all*/
        if (strcmp(as_file+len-3,".as") != 0){
            printf("Invalid file name, '.as' is missing.");
            continue;
        }

        /*If we got here, the file exists and also has the right extension,
          we will try to open the file for reading*/
        orig_file = fopen(as_file, "r");

        /*If we did not manage to open the file, we print a warning message that
          the file was not opened and continue to the next file.*/
        if (orig_file == NULL){
            printf("Failed to open file for reading.");
            continue;
        }

        /*If we got all the way here, the file exists with the right extension
          and it was opened!*/
        /*After the pre-assembler, if there were errors the new file was not
          created, and if everything was fine the file was created.*/
        fclose(orig_file);
        change_extension(as_file, am_file, ".am");

        /* If FALSE was returned from the pre-assembler*/
        /*it means an error/errors were found in the pre-assembler stage, the files
          will not be written out, and the check will not continue to the next stages*/
        /*All of the errors are already printed inside the pre-assembler, and we
          move on to check the next file.*/
        if (!pre_assembler(as_file, am_file)){
            continue;
        }

        /**
         * If the first pass and the second pass succeeded,
         * we declare three strings to hold the future file names,
         * we call the function that changes the extension of the files for the
         * output files, and we call the function that creates the files, which
         * creates the ob file automatically and creates the files with the
         * ent and ext extensions only if needed.
         * After that it moves on to the next file.
         */

        /*We open the file that was created by the pre-assembler and run the first pass 
          on it, and after that the second pass*/
        am_fp = fopen(am_file, "r");
        if (am_fp == NULL){ continue; }
        first_pass_ok = first_pass(am_fp, am_file, &icf, &dcf);
        fclose(am_fp);
        if (first_pass_ok && second_pass(am_file)){

            char ent_file[256], ext_file[256], ob_file[256];
        
            change_extension(as_file, ob_file, ".ob");
            change_extension(as_file, ent_file, ".ent");
            change_extension(as_file, ext_file, ".ext");

            write_output_files(ob_file, ent_file, ext_file);

            continue;
        }

        /*Otherwise, we delete the files that were created, close the file we
          worked on and continue to the next file.*/
        remove(am_file);

        /**
         * The check whether errors were found in the first and second pass is
         * done inside the second pass program,
         * if errors were found they are printed, the files are not saved and we
         * continue to the next file.
         * If everything was fine, the files are saved in the output directory
         * and we continue to the next file.
         */
    }

    return 1;

}
