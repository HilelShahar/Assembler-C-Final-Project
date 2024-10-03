/**
 * This is the utility header file.
 */
#ifndef UTILITY_H
#define UTILITY_H
#include <stdio.h>
#include "labels.h"

/* Memory node struct definition */
typedef struct Mem_Node {
    void *ptr;
    struct Mem_Node *next;
} Mem_Node;

/* Line struct definition */
typedef struct Line {
    FILE *file;          /* File pointer for closing file before exiting in case of a system error */
    char *file_am_name;  /* File name for printing errors */
    char *content;       /* Line content */
    int line_num;        /* Assembly code line number */
    Label *label;        /* Potential label declaration */
} Line;

/**
 * Allocates memory and tracks it in a linked list.
 * @param size The size of memory to allocate.
 * @return Pointer to the allocated memory, or NULL if allocation failed.
 */
void *allocate_memory(long size);


/**
 * Deallocates memory and removes it from the linked list.
 * @param ptr Pointer to the memory to deallocate.
 */
void deallocate_memory(void *ptr);


/**
 * Frees all allocated memory and clears the linked list.
 */
void free_all_memory();


/**
 * Searches for a file and opens it in read mode.
 * @param file_name The name of the file to search for.
 * @return Pointer to the opened file, or NULL if the file was not found.
 */
FILE *search_file(char *file_name);


/**
 * Deletes a file from the filesystem.
 * @param filename The name of the file to delete.
 */
void delete_file(char *filename);


/**
 * Validates and returns a file name with the ".as" extension appended.
 * @param file_name The original file name.
 * @return Pointer to the new file name with the ".as" extension, or NULL if an error occurred.
 */
char *valid_file_name(char *file_name);


/**
 * Adds an extension to a file name.
 * @param file_name The original file name.
 * @param extension The extension to append.
 * @return Pointer to the new file name with the extension, or NULL if an error occurred.
 */
char *add_extension(char *file_name,char *extension);


/**
 * Changes the extension of a file name.
 * @param file_name The original file name.
 * @param new_extension The new extension to append.
 * @return Pointer to the new file name with the new extension, or NULL if an error occurred.
 */
char *change_extension(char *file_name,char *new_extension);


/**
 * Trims leading and trailing whitespace from a string.
 * @param str The string to trim.
 * @return Pointer to the trimmed string.
 */
char *trim_whitespace(char* str);


/**
 * Checks if a string contains whitespace characters.
 * @param str The string to check.
 * @return 1 if whitespace is found, 0 otherwise.
 */
int contains_whitespace(char *str);


/**
 * Parses the numbers from the given line and returns them as an array.
 * @param line Pointer to the structure representing the current line of code being processed.
 * @param ptr The current position in the line.
 * @param num_count Pointer to an integer to store the count of numbers found.
 * @param errors_found Pointer to the error counter.
 * @return Pointer to an array of integers containing the parsed numbers.
 */
int *get_numbers(Line *line,char *ptr,int *num_count,int *errors_found);


/**
 * Gets the first word from a string.
 * @param str The string to extract the first word from.
 * @return Pointer to the first word, or NULL if memory allocation failed.
 */
char *get_first_word(char *str);


/**
 * Checks if a word is a standalone word in a string.
 * @param str The string to search in.
 * @param word The word to search for.
 * @return 1 if the word is found, 0 otherwise.
 */
int is_standalone_word(char *str,char *word);


/**
 * Creates a new line struct and initializes its fields.
 * @param file The file pointer associated with the line.
 * @param file_am_name The name of the file being processed.
 * @param content The content of the line.
 * @param line_num The line number in the file.
 * @return Pointer to the newly created line struct, or NULL if memory allocation failed.
 */
Line *create_line(FILE *file,char *file_am_name,char *content,int line_num);
/**
 * Frees the memory allocated for a line struct.
 * @param line Pointer to the line struct to free.
 */
void free_line(Line *line);


/**
 * Creates an object file (.ob) with machine code.
 * @param file_ob_name The name of the object file to create.
 * @param code Array containing the instruction code.
 * @param data Array containing the data code.
 * @param IC Pointer to the instruction counter.
 * @param DC Pointer to the data counter.
 */
void create_ob_file(char *file_ob_name,unsigned short *code,unsigned short *data,int *IC,int *DC);


/**
 * Creates an entry file (.ent) with entry labels.
 * @param file_ent_name The name of the entry file to create.
 */
void create_ent_file(char *file_ent_name);


/**
 * Creates an external file (.ext) with external labels.
 * @param file_ext_name The name of the external file to create.
 */
void create_ext_file(char *file_ext_name);


#endif