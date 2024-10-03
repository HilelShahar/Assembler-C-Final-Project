/**
 * This is the second pass file, which is the last step in the assembler process.
 * This file processes and updates uncoded label addresses, creates the relevant
 * output files (.ob, .ent, .ext), and manages potential errors.
 */
#include "../Header Files/second_pass.h"
#include <stdio.h>
#include <stdlib.h>
#include "../Header Files/definitions.h"
#include "../Header Files/errors.h"
#include "../Header Files/analyzer.h"

int second_pass(char *file_am_name, unsigned short *code, unsigned short *data, int *IC, int *DC) {
    char *file_ob_name, *file_ent_name, *file_ext_name;
    int errors_found = 0;

    /* Checking if all "entry" labels were defined */
    if (check_entry_labels(file_am_name) != 0) {
        errors_found = 1;  /* Indicates faliure */
    }
    /* Handling uncoded label addresses */
    update_data_labels(IC);
    if (code_operand_labels(file_am_name,code,IC) != 0) {
        free_labels();
        free_all_memory();
        return 1;  /* Indicates faliure */
    }
    /* Getting the object file name */
    file_ob_name = change_extension(file_am_name,".ob");

    /* Creating the object file */
    create_ob_file(file_ob_name,code,data,IC,DC);

    /* Creating "file.ent" if there are "entry" labels */
    if (entry_exist() != 0) {
        file_ent_name = change_extension(file_am_name,".ent");
        create_ent_file(file_ent_name);
        deallocate_memory(file_ent_name);
    }
    /* Creating "file.ext" if there are "extern" labels */
    if (extern_exist() != 0) {
        file_ext_name = change_extension(file_am_name,".ext");
        create_ext_file(file_ext_name);
        deallocate_memory(file_ext_name);
    }
    deallocate_memory(file_ob_name);
    free_labels();
    printf("* Second pass was successful\n");
    return errors_found;
}

int code_operand_labels(char *file_am_name, unsigned short *code, int *IC) {
    int errors_found = 0, i = 0, j = 0;
    Label *opernad_label, *label;
    unsigned short word = 0;

    /* Looping through code array */
    while (i <= *IC) {
        while ((code[j] & BIT_MASK_SIGNAL) != BIT_MASK_SIGNAL)  /* Searching for uncoded label addresses signaled by the first pass */
            j++;

        opernad_label = get_opernad_label();  /* Getting the next label of type "operand" */
        if (opernad_label == NULL) {
            return errors_found;  /* Indicates no more labels of type "operand" left */
        }

        if ((label = is_label_defined(opernad_label->name)) != NULL) {  /* Checking if this label was defined */
            word |= (unsigned short)(label->address & MASK_12BIT);
            word <<= BIT_MASK_SIGNAL;  /* Setting bits 3-14 */

            if (label->type == EXTERN && label->location == TBD) {  /* Indicates operand label is of type "extern" */
                word |= BIT_MASK_EXTERNAL;  /* Setting bit 0 for "External" */

                if (add_label(label->name,opernad_label->address+STARTING_ADDRESS,EXTERN,CODE) == NULL) {  /* Indicates memory allocation failed */
                    free_labels();
                    free_all_memory();
                    exit(1);  /* Exiting program */
                }
            } else if (label->type != EXTERN) {  /* Indicates operand label is of type "entry" */
                word |= BIT_MASK_RELOCATABLE;  /* Setting bit 1 for "Relocatable" */
            }

            code[j] = word;  /* Updating machine code */
        } else {
            print_specific_error(Error_69,file_am_name,code[j]>>BIT_MASK_SIGNAL,opernad_label->name);
            errors_found = 1;  /* Indicates faliure */
        }
        remove_label(opernad_label);
        word = 0;  /* Resetting word */
        i++;
        j++;
    }
    return errors_found;
}