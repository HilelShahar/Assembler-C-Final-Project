/**
 * This file is the utility file that provides general support for the assembler project.
 * It contains helper functions that handle string manipulation, memory management, and other general tasks.
 * These functions are used throughout the project and ensure code reusability.
 * This file defines a static head variable to simplify memory management and error handling and avoiding the need to pass head as a parameter
 * to functions, allowing easy access to the memory nodes linked list for cleanup in case of errors, while keeping the variable encapsulated within the file.
 */
#include "../Header Files/utility.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header Files/definitions.h"
#include "../Header Files/errors.h"
#include "../Header Files/labels.h"
#include "../Header Files/macros.h"

/* Defining the head of the memory-nodes linked list */
static Mem_Node *head = NULL;

void *allocate_memory(long size) {
    Mem_Node *new_node;
    void *ptr = malloc(size);  /* Using void * for compatibility with different data types */
    if (ptr == NULL) {
        print_system_error(Error_1);
        free_macros();
        free_labels();
        free_all_memory();
        return NULL;
    }
    new_node = (Mem_Node *)malloc(sizeof(Mem_Node));
    if (new_node == NULL) {
        print_system_error(Error_1);
        free_macros();
        free_labels();
        free_all_memory();
        return NULL;
    }
    new_node->ptr = ptr;
    new_node->next = head;
    head = new_node;
    return ptr;
}

void deallocate_memory(void *ptr) {
    Mem_Node *current = head;
    Mem_Node *prev = NULL;

    while (current != NULL) {
        if (current->ptr == ptr) {
            if (prev == NULL) {
                head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->ptr);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void free_all_memory() {
    Mem_Node *current = head;
    Mem_Node *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp->ptr);
        free(temp);
    }
    head = NULL;
}

FILE *search_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("\n [CODE_3] | ERROR | File \"%s\" was not found",filename);
        free_all_memory();
        return NULL;
    }
    fclose(file);
    return file;
}

void delete_file(char *filename) {
    if (remove(filename) != 0)
        print_system_error(Error_4);
}

char *valid_file_name(char *filename) {
    /* Appending the ".as" extension */
    char *new_file_name = add_extension(filename,".as");
    if (new_file_name == NULL)
        return NULL;
    return new_file_name;
}

char *add_extension(char *filename, char *extension) {
    char *new_filename;
    long filename_len = strlen(filename);
    long extension_len = strlen(extension);

    /* Checking if the file name already has the ".as" extention */
    if (extension_len < filename_len) {
        if (strcmp(filename + filename_len - extension_len, extension) == 0) {
            print_system_error(Error_2);
            return NULL;
        }
    }
    /* Allocating memory for the new filename */
    new_filename = (char *)allocate_memory(filename_len + extension_len + 1);  /* +1 for the null terminator */
    if (new_filename == NULL)  /* Indicates memory allocation failed */
        exit(1);  /* Exiting program */

    /* Copying the original filename and appending the extension */
    strcpy(new_filename, filename);
    strcat(new_filename, extension);
    return new_filename;
}

char *change_extension(char *file_name, char *new_extension) {
    /* Finding the last occurrence of the '.' character */
    char *dot = strrchr(file_name,DOT);

    /* Calculating the length for the filename with the new extension */
    int base_length = dot - file_name;
    int new_extension_length = strlen(new_extension);
    int new_filename_length = base_length + new_extension_length + 1;  /* +1 for the null terminator */

    /* Allocating memory for the new filename */
    char *new_filename = (char *)allocate_memory(new_filename_length);
    if (new_filename == NULL)  /* Indicates memory allocation failed */
        exit(1);  /* Exiting program */

    /* Copying the base part of the original filename */
    strncpy(new_filename, file_name, base_length);
    new_filename[base_length] = NULL_TERMINATOR;  /* Null-terminating the base part */

    /* Appending the new extension */
    strcat(new_filename, new_extension);
    return new_filename;
}

char *trim_whitespace(char *str) {
    char *end;

    while (*str && isspace(*str))  /* Incrementing the pointer 'str' while the current character is a whitespace */
        str++;

    if (*str == NULL_TERMINATOR)  /* If the string is all whitespaces or empty, returning the original string */
        return str;

    end = str + strlen(str) - 1;  /* Setting 'end' to point to the last character */

    /* Moving 'end' backward to the last non whitespace character */
    while (end > str && isspace(*end))
        end--;

    *(end + 1) = NULL_TERMINATOR;  /* Null-terminating the string */

    return str;
}

int contains_whitespace(char *str) {
    while (*str != NULL_TERMINATOR) {
        if (isspace(*str)) {
            return 1;  /* Indicates whitespace character found */
        }
        str++;
    }
    return 0;  /* Indicates no whitespace character found */
}

int *get_numbers(Line *line, char *ptr, int *num_count, int *errors_found) {
    int *result;
    char buffer[BUFFER_SIZE];
    int numbers[MAX_DATA_NUM];
    int temp_count = 0, i = 0, last_was_comma = 0, length = strlen(ptr), num, j;

    while (i < length && isspace(ptr[i]))  /* Skipping leading whitespace */
        i++;

    if (!isdigit(ptr[i]) && ptr[i] != MINUS && ptr[i] != PLUS) {  /* Checking for an invalid character */
        *errors_found = 1;
        if (ptr[i] == COMMA) {
            print_syntax_error(Error_34,line->file_am_name,line->line_num);
      	    return NULL;
      	}
        print_syntax_error(Error_35,line->file_am_name,line->line_num);
        return NULL;
    }
    while (i < length) {
        while (i < length && isspace(ptr[i]))  /* Skipping whitespace characters */
            i++;
        if (i >= length)
            break;
        /* Checking for a number with an optional sign */
        if (isdigit(ptr[i]) || ((ptr[i] == MINUS || ptr[i] == PLUS) && isdigit(ptr[i + 1]))) {
            last_was_comma = 0;
            j = 0;

            if (ptr[i] == MINUS || ptr[i] == PLUS)  /* Handling optional sign */
                buffer[j++] = ptr[i++];
            while (i < length && isdigit(ptr[i]))  /* Getting the number */
                buffer[j++] = ptr[i++];

            /* Checking for an invalid character after the number */
            if (i < length && !isspace(ptr[i]) && ptr[i] != COMMA && ptr[i] != MINUS && ptr[i] != PLUS) {
                print_syntax_error(Error_35,line->file_am_name,line->line_num);
                *errors_found = 1;
                return NULL;
            }
            buffer[j] = NULL_TERMINATOR;
            num = atoi(buffer);

            /* Checking if the number is in range */
            if (num < MIN_15BIT || num > MAX_15BIT) {
                print_syntax_error(Error_39,line->file_am_name,line->line_num);
                *errors_found = 1;
                return NULL;
            }
            /* Adding the number to the array */
            numbers[temp_count] = num;
            temp_count++;

            while (i < length && isspace(ptr[i]))  /* Skipping whitespace characters after the number */
                i++;

            if (i < length && ptr[i] == COMMA) {  /* Checking for a comma after the number */
                last_was_comma = 1;
                i++;
            } else if (i < length && ptr[i] != COMMA) {
                print_syntax_error(Error_36,line->file_am_name,line->line_num);
                *errors_found = 1;
                return NULL;
            }
        } else {
            print_syntax_error(Error_35,line->file_am_name,line->line_num);
            *errors_found = 1;
            return NULL;
        }
        /* Checking for multiple consecutive commas */
        if (last_was_comma) {
            while (i < length && isspace(ptr[i]))
                i++;

            if (i < length && ptr[i] == COMMA) {
                print_syntax_error(Error_37,line->file_am_name,line->line_num);
                *errors_found = 1;
                return NULL;
            }
        }
    }
    if (last_was_comma) {
        print_syntax_error(Error_38,line->file_am_name,line->line_num);
        *errors_found = 1;
        return NULL;
    }
    result = (int *)allocate_memory(temp_count*sizeof(int));
    if (result == NULL) {  /* Indicates memory allocation failed (all other allocations were freed inside function) */
        fclose(line->file);
        free_line(line);
        exit(1);  /* Exiting program */
    }
    memcpy(result,numbers,temp_count*sizeof(int));
    *num_count = temp_count;

    return result;
}

char *get_first_word(char *str) {
    char *first_word;
    /* Finding the length of the first word */
    int length = 0;
    while (str[length] != NULL_TERMINATOR && !isspace(str[length])) {
        length++;
    }
    /* Allocating memory for the first word */
    first_word = (char *)allocate_memory((length+1)*sizeof(char));
    if (first_word == NULL)
        return NULL;  /* Indicates memory allocation failed */

    strncpy(first_word, str, length);   /* Copying the first word to the allocated memory */
    first_word[length] = NULL_TERMINATOR;  /* Null-terminating the string */

    return first_word;
}

int is_standalone_word(char *str, char *word) {
    int word_len = strlen(word);
    char *ptr = strstr(str,word);

    while (ptr != NULL) {  /* Checking if the character before the word is a space or the start of the string */
        if ((ptr == str || isspace(ptr[-1])) &&
            /* Checking if the character after the word is a space or the end of the string */
            (isspace(ptr[word_len]) || ptr[word_len] == '\0')) {
            return 1;  /* Indicates word was found */
            }
        /* Moving to the next occurrence of the word */
        ptr = strstr(ptr+word_len,word);
    }
    return 0;  /* Indicates word was not found */
}

Line *create_line(FILE *file, char *file_am_name, char *content, int line_num) {
    Line *new_line = (Line *)malloc(sizeof(Line));
    if (new_line == NULL) {
        print_system_error(Error_1);
        return NULL;  /* Indicates failure */
    }
    /* Allocating memory for the file name and the line's content */
    new_line->file_am_name = (char *)malloc(strlen(file_am_name)+1);
    if (new_line->file_am_name == NULL) {
        print_system_error(Error_1);
        free(new_line);
        return NULL;  /* Indicates failure */
    }
    strcpy(new_line->file_am_name, file_am_name);

    new_line->content = (char *)malloc(strlen(content)+1);
    if (new_line->content == NULL) {
        print_system_error(Error_1);
        free(new_line->file_am_name);
        free(new_line);
        return NULL;  /* Indicates failure */
    }
    strcpy(new_line->content, content);
    /* Setting file pointer and line number */
    new_line->file = file;
    new_line->line_num = line_num;
    new_line->label = NULL;
    return new_line;  /* Indicates success */
}

void free_line(Line *line) {
    if (line->file_am_name != NULL)
        free(line->file_am_name);

    if (line->content != NULL)
        free(line->content);

    free(line);
}

void create_ob_file(char *file_ob_name, unsigned short *code, unsigned short *data, int *IC, int *DC) {
    FILE *file_ob = fopen(file_ob_name,"w");
    int i = 0, j = 0;;

    if (file_ob == NULL) {  /* Failed to open file for writing */
        print_system_error(Error_6);
        free_labels();
        free_all_memory();
        exit(1);  /* Exiting program */
    }

    /* Writing header into file */
    fprintf(file_ob,"  %d %d\n",*IC,*DC);

    /* Writing machine code into file */
    for (; i<*IC; i++) {
        fprintf(file_ob,"%04d %05o\n",i+STARTING_ADDRESS,code[i]);
    }
    for (; j<*DC; j++) {
        fprintf(file_ob,"%04d %05o\n",j+i+STARTING_ADDRESS,data[j]);
    }

    fclose(file_ob);
}

void create_ent_file(char *file_ent_name) {
    FILE *file_ent = fopen(file_ent_name,"w");
    Label *current;

    if (file_ent == NULL) {  /* Failed to open file for writing */
        print_system_error(Error_6);
        free_labels();
        free_all_memory();
        exit(1);  /* Exiting program */
    }

    current = get_label_head();
    while (current != NULL) {
        if (current->type == ENTRY) {
            fprintf(file_ent,"%s %04d\n",current->name,current->address);
        }
        current = current->next;
    }
    fclose(file_ent);
}

void create_ext_file(char *file_ext_name) {
    FILE *file_ext = fopen(file_ext_name,"w");
    Label *current;

    if (file_ext == NULL) {  /* Failed to open file for writing */
        print_system_error(Error_6);
        free_labels();
        free_all_memory();
        exit(1);  /* Exiting program */
    }

    current = get_label_head();
    while (current != NULL) {
        if (current->type == EXTERN && current->location == CODE) {
            fprintf(file_ext,"%s %04d\n",current->name,current->address);
        }
        current = current->next;
    }
    fclose(file_ext);
}