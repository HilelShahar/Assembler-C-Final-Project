/**
 * This is the macros handling file, which manages the creation, modification, and deletion of
 * macros in the assembler process, as well as the handling of dynamic allocations.
 * It ensures proper memory management and error handling for macros used in the assembly process.
 * This file defines a static head variable to simplify memory management and error handling and avoiding the need to pass head as a parameter
 * to functions, allowing easy access to the macros linked list for cleanup in case of errors, while keeping the variable encapsulated within the file.
 */
#include "../Header Files/macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header Files/definitions.h"
#include "../Header Files/errors.h"

/* Defining the head of the macros linked list */
static Macro *head = NULL;

int add_macro(char *name, int line) {
    Macro *last_macro;

    Macro *new_macro = (Macro *)malloc(sizeof(Macro));
    if (new_macro == NULL) {
        print_system_error(Error_1);
        return 1;  /* Indicates failure */
    }
    /* Allocating memory and copying the name */
    new_macro->name = (char *)malloc(strlen(name)+1);  /* +1 to accommodate '\0' */
    if (new_macro->name == NULL) {
        print_system_error(Error_1);
        free(new_macro);  /* Freeing the previously allocated macro */
        return 1;  /* Indicates failure */
    }
    strcpy(new_macro->name,name);

    /* Setting the content to NULL */
    new_macro->content = NULL;

    /* Setting the line number and the next pointer */
    new_macro->line = line;
    new_macro->next = NULL;

    /* If the list is empty, setting the new macro as the head */
    if (head == NULL) {
        head = new_macro;
    } else {
        /* Otherwise, finding the end of the list and adding the new macro */
        last_macro = get_last_macro();
        last_macro->next = new_macro;
    }
    return 0;  /* Indicates success */
}

Macro *is_macro_name(char *macro_name) {
    Macro *current = head;

    while (current != NULL) {
        if (strcmp(current->name, macro_name) == 0) {
            return current;  /* Indicates name is a macro name and returns a pointer to its node */
        }
        current = current->next;
    }
    return NULL;  /* Indicates name is not a macro name */
}

int append_macro_content(char *new_content) {
    Macro *current;
    int current_length, new_content_length, total_length;
    char *new_memory;

    current = get_last_macro(head);  /* Skipping to the last macro in the list */

    /* Current cannot be NULL because append_macro_content is called only if a macro node was created - the list is not empty */
    if (current->content != NULL) {
        current_length = strlen(current->content);
    } else {
        current_length = 0;
    }

    new_content_length = strlen(new_content);
    total_length = current_length + new_content_length + 1;  /* +1 to accommodate '\0' */

    new_memory = realloc(current->content,total_length);  /* Reallocating or allocating memory for the new content */
    if (new_memory == NULL) {
        print_system_error(Error_1);
        return 1;  /* Indicates faliure */
    }

    current->content = new_memory;

    if (current_length == 0) {  /* If the current content is NULL, initializing it as an empty string */
        current->content[0] = NULL_TERMINATOR;
    }

    strcat(current->content,new_content);  /* Appending the new content to the existing content */

    return 0;  /* Indicates success */
}

Macro *get_last_macro() {
    Macro *current;

    if (head == NULL)
        return NULL;  /* Indicates list is empty */

    current = head;
    while (current->next != NULL)
        current = current->next;

    return current;  /* Returning the last macro in the list */
}

void remove_last_macro() {
    Macro *current;

    if (head->next == NULL) {  /* Only one macro in the list */
        free(head->name);
        if (head->content)  /* Checking if content is NULL in case of "Error_17" */
            free(head->content);
        free(head);
        head = NULL;
        return;
    }
    current = head;

    while (current->next->next != NULL) {   /* Skipping to the second-to-last macro */
        current = current->next;
    }

    free(current->next->name);
    if (current->next->content)  /* Checking if content is NULL in case of "Error_17" */
        free(current->next->content);
    free(current->next);

    current->next = NULL;  /* Updating the second-to-last macro to be the new last */
}

void free_macros() {
    Macro *current = head;
    Macro *next;

    while (current != NULL) {
        next = current->next;  /* Updating the next pointer */

        free(current->name);  /* Freeing the dynamically allocated name */
        free(current->content);  /* Freeing the dynamically allocated content */
        free(current);  /* Freeing the macro node itself */

        current = next;  /* Moving to the next node */
    }
    head = NULL;
}