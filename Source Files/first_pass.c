/**
 * This is the first pass file, which is the first step in the assembler analysis process.
 * This file handles the scanning and storing of instructions, operations, and labels,
 * converting them into machine code while handling all types off potential errors.
 * If no errors are detected, it proceeds with the second pass.
 */
#include "../Header Files/first_pass.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header Files/definitions.h"
#include "../Header Files/errors.h"
#include "../Header Files/labels.h"
#include "../Header Files/macros.h"
#include "../Header Files/second_pass.h"
#include "../Header Files/utility.h"
#include "../Header Files/analyzer.h"

int first_pass(char *file_name) {
    unsigned short code[CAPACITY] = {0}, data[CAPACITY] = {0};  /* Initializing machine code arrays */
    int IC = 0, DC = 0;

    /* Getting the new file name */
    char *file_am_name = change_extension(file_name,".am");

    /* Scanning the file */
    if (scan_text(file_am_name,code,data,&IC,&DC) != 0) {
        free_labels();
        free_macros();
        free_all_memory();
        return 1;  /* Indicates faliure */
    }
    free_macros();  /* Macros are no longer needed */

    printf("* First pass was successful\n");

    /* Starting second pass */
    if (second_pass(file_am_name,code,data,&IC,&DC) != 0) {
        free_labels();
        free_all_memory();
        return 1;  /* Indicates faliure */
    }
    deallocate_memory(file_am_name);
    return 0;  /* Indicates success */
}

int scan_text(char *file_am_name, unsigned short *code, unsigned short *data, int *IC, int *DC) {
    char temp[MAX_LINE_LENGTH+1];  /* +1 to accommodate '\0' */
    int Usage = 0, errors_found = 0, line_count = 0;
    char *trimmed_line;
    Line *line;

    FILE *file_am = fopen(file_am_name,"r");
    if (file_am == NULL) {  /* Failed to open file for reading */
        print_system_error(Error_5);
        free_macros();
        free_all_memory();
        exit(1);  /* Exiting program */
    }
    /* Reading line by line */
    while (fgets(temp,MAX_LINE_LENGTH+1,file_am)) {
        line_count++;

        /* Checking if the current line a comment */
        if (temp[0] == COMMENT)
            continue;  /* Skipping to the next line */

        /* Trimming leading and trailing whitespace characters */
        trimmed_line = trim_whitespace(temp);

        /* Checking if the current line is a empty */
        if (strlen(trimmed_line) == 0)
            continue;  /* Skipping to the next line */

        line = create_line(file_am,file_am_name,trimmed_line,line_count);
        if (line == NULL) {
            fclose(file_am);
            free_labels();
            free_macros();
            free_all_memory();
            exit(1);  /* Exiting program */
        }
        scan_word(code,data,&Usage,IC,DC,line,&errors_found);
        free_line(line);
    }
    fclose(file_am);
    return errors_found;
}

void scan_word(unsigned short *code, unsigned short *data, int *Usage, int *IC, int *DC, Line *line, int *errors_found) {
    char *ptr = line->content;
    char *current_word, *temp;
    int curr_word_len, len, res;
    Label *label;

    /* Getting the first word */
    current_word = get_first_word(line->content);
    if (current_word == NULL) {  /* Indicates memory allocation failed (all the other allocations were freed inside function) */
        fclose(line->file);
        free_line(line);
        exit(1);  /* Exiting program */
    }
    curr_word_len = strlen(current_word);

    /* Checking for a potential label definition */
    if (current_word[curr_word_len-1] == COLON) {
        res = valid_label_name(current_word,REGULAR,line,errors_found);
        if (res == 0) {
            label = add_label(current_word,0,REGULAR,TBD);  /* Adding label to the list */
        } else if (res == -1) {  /* Special case */
            label = add_label(current_word,0,ENTRY,TBD);  /* Adding entry label to the list */
        } else {
            deallocate_memory(current_word);
            return;  /* Scanning line finished */
        }
        if (label == NULL) {  /* Indicates memory allocation failed */
            fclose(line->file);
            free_line(line);
            free_labels();
            free_macros();
            free_all_memory();
            exit(1);  /* Exiting program */
        }
        line->label = label;  /* Updating struct line label pointer */
        deallocate_memory(current_word);  /* Freeing memory no longer needed */
    }
    /* Scanning the next word */
    if (line->label != NULL) {
        if (contains_whitespace(ptr)) {
            while (*ptr != NULL_TERMINATOR && !isspace(*ptr))  /* Skipping the label name */
                ptr++;
            while (*ptr != NULL_TERMINATOR && isspace(*ptr))  /* Setting 'ptr' to point to the next word */
                ptr++;
            /* Getting the next word */
            current_word = get_first_word(ptr);
            if (current_word == NULL) {  /* Indicates memory allocation failed (all the other allocations were freed inside function) */
                fclose(line->file);
                free_line(line);
                exit(1);  /* Exiting program */
            }
        } else {
            ptr[strlen(ptr)-1] = NULL_TERMINATOR;  /* Getting the label name withougt the colon (:) for th error message */
            print_syntax_error(Error_22,line->file_am_name,line->line_num);
            *errors_found = 1;
            return;  /* Scanning line finished */
        }
    }
    /* Checking for a potential instruction */
    if (is_instruction(data,Usage,DC,line,ptr,current_word,errors_found) != 0) {
        deallocate_memory(current_word);
        return;  /* Scanning line finished */
    }

    /* Checking for a potential operation */
    if (is_operation(code,Usage,IC,line,ptr,current_word,errors_found) != 0) {
        deallocate_memory(current_word);
        return;  /* Scanning line finished */
    }

    /* Handling special cases */
    if (is_macro_name(current_word) != NULL) {  /* Checking for a failed macro call */
        print_specific_error(Error_33,line->file_am_name,line->line_num,current_word);
        *errors_found = 1;
        deallocate_memory(current_word);
        return;  /* Scanning line finished */
    }
    if (strchr(current_word+1,COLON) != NULL) {  /* Checking for a failed label declaration */
        print_specific_error(Error_65,line->file_am_name,line->line_num,current_word);
        *errors_found = 1;
        deallocate_memory(current_word);
        return;  /* Scanning line finished */
    }
    while (ptr && !isspace(*ptr))  /* Skipping the first word */
        ptr++;
    while (ptr && isspace(*ptr))  /* Skipping whitespace characters */
        ptr++;
    if (ptr && *ptr == COLON) {  /* Checking for a failed label declaration */
        print_specific_error(Error_66,line->file_am_name,line->line_num,current_word);
        *errors_found = 1;
        deallocate_memory(current_word);
        return;  /* Scanning line finished */
    }
    if (is_label_name(current_word) != NULL) {  /* Checking for a label at the start of the line */
        print_specific_error(Error_71,line->file_am_name,line->line_num,current_word);
        *errors_found = 1;
        deallocate_memory(current_word);
        return;  /* Scanning line finished */
    }
    len = strlen(current_word) + TWO;  /* +1 for dot, +1 for null-terminator */
    temp = (char *)allocate_memory(len);
    if (temp == NULL) {
        fclose(line->file);
        free_line(line);
        exit(1);  /* Exiting program */
    }
    temp[0] = DOT;  /* Adding the dot at the beginning */
    strcpy(temp+1,current_word);  /* Copying the original word after the dot */
    if (which_instr(temp) != -1) {
        print_specific_error(Error_67,line->file_am_name,line->line_num,current_word);
        *errors_found = 1;
        deallocate_memory(temp);
        deallocate_memory(current_word);
        return;  /* Scanning line finished */
    }
    deallocate_memory(temp);
    print_specific_error(Error_68,line->file_am_name,line->line_num,current_word);
    *errors_found = 1;
    deallocate_memory(temp);
    deallocate_memory(current_word);
}