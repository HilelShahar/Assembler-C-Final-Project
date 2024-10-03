/**
 * This is the labels handling file of the assembler that includes functions
 * to add, search, and manage labels in a linked list, and also to update addresses and free the allocated memory.
 * It handles different types of labels such as regular, entry, or external.
 * This file defines a static head variable to simplify memory management and error handling and avoiding the need to pass head as a parameter
 * to functions, allowing easy access to the labels linked list for cleanup in case of errors, while keeping the variable encapsulated within the file.
 */
#include "../Header Files/labels.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header Files/definitions.h"
#include "../Header Files/errors.h"

/* Defining the head of the labels linked list */
static Label *head = NULL;

Label *add_label(char *name, int address, Type type, Location location) {
    Label *last_label;

    Label *new_label = (Label *)malloc(sizeof(Label));
    if (new_label == NULL) {
        print_system_error(Error_1);
        return NULL;  /* Indicates failure */
    }
    /* Allocating memory and copying the name */
    new_label->name = (char *)malloc(strlen(name)+1);  /* +1 to accommodate '\0' */
    if (new_label->name == NULL) {
        print_system_error(Error_1);
        free(new_label);
        return NULL;  /* Indicates failure */
    }
    strcpy(new_label->name,name);

    /* Setting the address, type, location and next pointer */
    new_label->address = address;
    new_label->type = type;

    if (location != TBD) {
        new_label->location = location;
    } else {
        new_label->location = TBD;
    }
    new_label->next = NULL;

    /* If the list is empty, setting the new label as the head */
    if (head == NULL) {
        head = new_label;
    } else {
        /* Otherwise, finding the end of the list and adding the new label */
        last_label = get_last_label();
        last_label->next = new_label;
    }
    return new_label;  /* Indicates success */
}

Label *is_label_name(char *label_name) {
    Label *current = head;

    while (current != NULL) {
        if (strcmp(current->name,label_name) == 0 && current->type != OPERAND) {
            return current;  /* Indicates name is a label name and returns a pointer to its node */
        }
        current = current->next;
    }
    return NULL;  /* Indicates name is not a label name */
}

Label *is_label_defined(char *label_name) {
    Label *current = head;

    while (current != NULL) {
        if (strcmp(current->name,label_name) == 0 &&  /* Determining if label was defined based on location and type */
            ((current->type != EXTERN && current->location != TBD) || (current->type == EXTERN && current->location == TBD))) {
            return current;  /* Indicates name is a label name and returns a pointer to its node */
        }
        current = current->next;
    }
    return NULL;  /* Indicates name is not a label name */
}

int check_entry_labels(char *file_am_name) {
    Label *current = head;
    int errors_found = 0;

    while (current != NULL) {
        if (current->type == ENTRY && current->location == TBD) {  /* Checking for an undefined "entry" label */
            printf(" [CODE_7] | ERROR | File \"%s\" | Label \"%s\"",file_am_name,current->name);
            print_system_error(Error_72);
            errors_found = 1;  /* Indicates not all "entry" labels were defined */
        }
        current = current->next;
    }
    return errors_found;
}

void update_data_labels(int *IC) {
    Label *current = head;

    while (current != NULL) {
        if (current->location == DATA) {
            current->address += *IC + STARTING_ADDRESS;
        }
        current = current->next;
    }
}

Label *get_opernad_label() {
    Label *current = head;

    while (current != NULL) {
        if (current->type == OPERAND) {
            return current;  /* Indicates an "operand" type label was found */
        }
        current = current->next;
    }
    return NULL;  /* Indicates no "operand" type label was found */
}

int entry_exist() {
    Label *current = head;

    while (current != NULL) {
        if (current->type == ENTRY) {
            return 1;  /* Indicates an "entry" type label was found */
        }
        current = current->next;
    }
    return 0;  /* Indicates no "entry" type label was found */

}

int extern_exist() {
    Label *current = head;

    while (current != NULL) {
        if (current->type == EXTERN) {
            return 1;  /* Indicates an "extern" type label was found */
        }
        current = current->next;
    }
    return 0;  /* Indicates no "extern" type label was found */

}

Label *get_label_head() {
    return head;
}

Label *get_last_label() {
    Label *current;

    if (head == NULL)
        return NULL;  /* Indicates list is empty */

    current = head;
    while (current->next != NULL)
        current = current->next;

    return current;  /* Returning the last label in the list */
}

void remove_last_label() {
    Label *current;

    if (head->next == NULL) {  /* Indicates only one label in the list */
        free(head->name);
        free(head);
        head = NULL;
        return;
    }
    current = head;

    while (current->next->next != NULL) {   /* Skipping to the second-to-last label */
        current = current->next;
    }
    free(current->next->name);
    free(current->next);

    current->next = NULL;  /* Updating the second-to-last label to be the new last */
}

void remove_label(Label *label) {
    Label *current = head;
    Label *prev = NULL;

    while (current != NULL) {
        if (current == label) {
            if (prev == NULL) {  /* Checking if "head" is the label to be removed */
                head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->name);
            free(current);
            return;  /* Label found and removed */
        }
        prev = current;
        current = current->next;
    }
}

void free_labels() {
    Label *current = head;
    Label *next;

    while (current != NULL) {
        next = current->next;  /* Updating the next pointer */

        free(current->name);  /* Freeing the dynamically allocated name */
        free(current);  /* Freeing the macro node itself */

        current = next; /* Moving to the next node */
    }
    head = NULL;
}