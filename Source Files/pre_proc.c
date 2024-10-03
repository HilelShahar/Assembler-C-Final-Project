/**
 * This is the pre-processing file, which is the initial step in the assembler process.
 * This file handles scanning and storing of macros while checking for declaration errors.
 * If no errors are detected, a new output file is created, where all macro calls are replaced
 * with their corresponding content, and all of the original macro declarations are removed.
 */
#include "../Header Files/pre_proc.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header Files/definitions.h"
#include "../Header Files/errors.h"
#include "../Header Files/macros.h"
#include "../Header Files/utility.h"
#include "../Header Files/analyzer.h"

int pre_processing(char *file_name) {
    /* Getting the new file name */
    char *file_am_name = change_extension(file_name,".am");

    /* Handling all macro calls and declarations */
    if (handle_macros(file_name,file_am_name) != 0) {
        free_macros();
        free_all_memory();
        return 1;  /* Indicates faliure */
    }
    deallocate_memory(file_am_name);
    printf("* Pre-processing completed\n");
    return 0;  /* Indicates success */
}

int handle_macros(char *file_name, char *file_am_name) {
    char *macro_name, *trimmed_line;
    char line[MAX_LINE_LENGTH+1], copy[MAX_LINE_LENGTH+1];  /* +1 to accommodate '\0' */
    int errors_found = 0 , macro_found = 0, line_count = 0, name_is_valid = 0, decl_line, line_length, ch;
    FILE *file, *file_am;
    Macro *macro_ptr;

    file = fopen(file_name,"r");
    if (file == NULL) {  /* Failed to open file for reading */
        print_system_error(Error_5);
        free_all_memory();
        exit(1);  /* Exiting program */
    }
    file_am = fopen(file_am_name,"w");
    if (file_am == NULL) {  /* Failed to open file for writing */
        print_system_error(Error_6);
        fclose(file);
        free_all_memory();
        exit(1);  /* Exiting program */
    }
    /* Reading line by line */
    while (fgets(line,MAX_LINE_LENGTH+1,file)) {
        line_count++;
        line_length = strlen(line);

        /* Validating line length */
        if (line_length == MAX_LINE_LENGTH && line[MAX_LINE_LENGTH-1] != '\n') {
            if (is_standalone_word(line,"macr") != 0) {
                if (macro_found == 0) {
                    macro_found = 1;
                } else {
                    macro_found = 0;
                }
            }
            print_syntax_error(Error_7,file_name,line_count);
            errors_found = 1;
            while ((ch = fgetc(file)) != '\n' && ch != EOF);  /* Clearing the rest of the line from the buffer */
            continue;  /* Skipping to the next line */
        }
        /* Skipping to the next line if the current line is a comment */
        if (*line == COMMENT) {
            if (errors_found == 0)
                fputs(line,file_am);  /* Copying line into "file.am" */
            continue;  /* Skipping to the next line */
        }
        strcpy(copy,line);
        trimmed_line = trim_whitespace(line);  /* Trimming leading and trailing whitespace characters */

        if (*trimmed_line == COMMENT) {  /* Checking for an illegal comment */
            print_syntax_error(Error_3,file_name,line_count);
            errors_found = 1;
            continue;  /* Skipping to the next line */
        }
        /* Writing the macro content into "file.am" if a macro call was detected */
        if ((macro_ptr = is_macro_name(trimmed_line)) != NULL) {
            if (errors_found == 0) {
                fputs(macro_ptr->content,file_am);
                fputs("\n",file_am);
            }
            continue;  /* Skipping to the next line */
        }
        /* Checking if -endmacr- command had been reached */
        if (macro_found == 1) {
            if (is_standalone_word(trimmed_line,"endmacr") == 0) {  /* Writing the current line into macro content */
                if (name_is_valid == 1 && append_macro_content(copy) != 0) {  /* Indicates memory allocation failed */
                    fclose(file);
                    fclose(file_am);
                    delete_file(file_am_name);
                    free_macros();
                    free_all_memory();
                    exit(1);  /* Exiting program */
                }
                continue;  /* Skipping to the next line */
            }
            /* Handling -endmacr- command potential errors */
            if (strlen(trimmed_line) > ENDMACR_LENGTH) {
                print_syntax_error(Error_15,file_name,line_count);
                if (name_is_valid == 1)
                    remove_last_macro();
                errors_found = 1;
                macro_found = 0;
                name_is_valid = 0;
                continue;  /* Skipping to the next line */
            }
            if (name_is_valid == 1) {  /* Checking if content is empty */
                if (get_last_macro()->content == NULL || strlen(trim_whitespace(get_last_macro()->content)) == 0) {
                    print_syntax_error(Error_17,file_name,line_count);
                    remove_last_macro();
                    errors_found = 1;
                }
            }
            name_is_valid = 0;
            macro_found = 0;
            continue;  /* Skipping to the next line */
        }
        /* Checking for a potential macro declaration */
        if (is_standalone_word(trimmed_line,"macr") == 0) {
            if (errors_found == 0)
                fputs(copy,file_am);  /* Copying line into "file.am" */
            continue;  /* Skipping to the next line */
        }
        /* If this line had been reached then a macro declaration was found */
        macro_found = 1;
        decl_line = line_count;

        /* Validating the macro declaration */
        if (strlen(trimmed_line) > MACR_LENGTH) {
            macro_name = valid_macro_decl(file_name,trimmed_line,line_count);
            if (macro_name) {
                if (is_macro_name(macro_name) != NULL) {  /* Checking if the name had already been defined */
                    print_syntax_error(Error_14,file_name,line_count);
                    errors_found = 1;
                    name_is_valid = 0;
                    continue;  /* Skipping to the next line */
                }
                /* Adding a new macro to the linked list */
                if (add_macro(macro_name,decl_line) != 0) {  /* Indicates memory allocation failed */
                    fclose(file);
                    fclose(file_am);
                    delete_file(file_am_name);
                    free_macros();
                    free_all_memory();
                    exit(1);  /* Exiting program */
                }
            } else {
                errors_found = 1;
                continue;  /* Skipping to the next line */
            }
        } else {
            print_syntax_error(Error_8,file_name,line_count);
            errors_found = 1;
            macro_found = 1;
            name_is_valid = 0;
            continue;  /* Skipping to the next line */
        }
        name_is_valid = 1;
    }
    fclose(file);
    fclose(file_am);
    if (errors_found != 0)
        delete_file(file_am_name);
    return errors_found;
}

char *valid_macro_decl(char *file_name, char *decl, int line_count) {
    char *macro_name;

    /* Checking if the first word is "macr" */
    if (strncmp(decl,"macr",MACR_LENGTH) == 0 && isspace(decl[MACR_LENGTH])) {
        decl += MACR_LENGTH;  /* Move the pointer to the next word */
        macro_name = trim_whitespace(decl);

        if(valid_macro_name(file_name,macro_name,line_count) != 0)  /* Validating macro name */
           return NULL;  /* Indicates faliure */
    } else {
        print_syntax_error(Error_9,file_name,line_count);
        return NULL;  /* Indicates faliure */
    }
    return macro_name;
}