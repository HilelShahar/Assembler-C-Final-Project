/**
 * This is the labels header file.
 */
#ifndef LABELS_H
#define LABELS_H

typedef enum Type {
    REGULAR,
    ENTRY,
    EXTERN,
    OPERAND
} Type;

typedef enum Location {
    TBD,  /* To Be Determined */
    CODE,
    DATA
} Location;

/* Label struct definition */
typedef struct Label {
    char *name;
    int address;
    Type type;
    Location location;
    struct Label *next;
} Label;

/**
 * Adds a new Label to the end of the linked list.
 * @param name The name of the new label.
 * @param address The address of the new label in memory.
 * @param type The type of the new label.
 * @param location The location of the new label in the machine code.
 * @return 0 for a successful operation ,1 if errors were detected.
 */
Label *add_label(char *name,int address,Type type,Location location);


/**
 * Checks if a given name is a label name.
 * @param label_name The name to check.
 * @return Pointer to the label if found, NULL otherwise.
 */
Label *is_label_name(char *label_name);


/**
 * Checks if a given label name is defined.
 * @param label_name The name to check.
 * @return Pointer to the label if found, NULL otherwise.
 */
Label *is_label_defined(char *label_name);


/**
 * Checks if all entry type labels were defined.
 * @param file_am_name The name of the file being processed.
 * @return 1 if errors were found, 0 otherwise.
 */
int check_entry_labels(char *file_am_name);


/**
 * Updates the addresses of data labels.
 * @param IC Pointer to the instruction counter.
 */
void update_data_labels(int *IC);


/**
 * Retrieves the first "operand" label.
 * @return Pointer to the operand label if found, NULL otherwise.
 */
Label *get_opernad_label();


/**
 * Checks if any "entry" type labels exist.
 * @return 1 if an entry label exists, 0 otherwise.
 */
int entry_exist();


/**
 * Checks if any "extern" type labels exist.
 * @return 1 if an extern label exists, 0 otherwise.
 */
int extern_exist();


/**
 * Gets the head of the label list.
 * @return Pointer to the head of the label list.
 */
Label *get_label_head();


/**
 * Gets the last label in the linked list.
 * @return Pointer to the last label in the list.
 */
Label *get_last_label();


/**
 * Removes the last label in the linked list.
 */
void remove_last_label();


/**
 * Removes a specific label from the linked list.
 * @param label Pointer to the label to be removed.
 */
void remove_label(Label *label);


/**
 * Frees all the labels in the linked list.
 */
void free_labels();


#endif