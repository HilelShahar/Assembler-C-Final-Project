/**
 * This is the macros header file.
 */
#ifndef MACROS_H
#define MACROS_H

/* Macro struct definition */
typedef struct Macro {
    char *name;
    char *content;
    int line;
    struct Macro *next;
} Macro;

/**
 * Adds a new macro to the end of the linked list.
 * @param name The name of the new macro.
 * @param line The line number associated with the new macro.
 * @return 0 for a successful operation ,1 if errors were detected.
 */
int add_macro(char *name,int line);


/**
 * Checks if the given name is a macro name.
 * @param macro_name The name to check.
 * @return Pointer to the macro if found, NULL otherwise.
 */
Macro *is_macro_name(char *macro_name);


/**
 * Appends new content to the last macro in the linked list.
 * @param new_content The content to append.
 * @return 0 for a successful operation, 1 if errors were detected.
 */
int append_macro_content(char *new_content);


/**
 * Gets the last macro in the linked list.
 * @return Pointer to the last macro.
 */
Macro *get_last_macro();


/**
 * Removes the last macro from the linked list.
 */
void remove_last_macro();


/**
 * Frees all macros in the linked list.
 */
void free_macros();


#endif