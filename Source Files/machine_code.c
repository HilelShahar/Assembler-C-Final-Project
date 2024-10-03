/**
 * This file handles the machine-coding for the assembler and contains functions for
 * handling operands and adding data and instruction codes to their arrays.
 */
#include "../Header Files/machine_code.h"
#include <stdio.h>
#include <stdlib.h>
#include "../Header Files/definitions.h"
#include "../Header Files/errors.h"
#include "../Header Files/labels.h"
#include "../Header Files/macros.h"
#include "../Header Files/utility.h"
#include "../Header Files/analyzer.h"

void add_data_code(unsigned short *data, int *DC, int number) {
    /* Getting the 15-bit 2's complement binary representation of the number */
    unsigned short word = (unsigned short)(number & MASK_15BIT);

    /* Adding the code to the data array */
    data[*DC] = word;

    (*DC)++;  /* Incrementing data count */
}

void add_instruction_code(unsigned short *code, int *Usage, int *IC, unsigned short word, int *errors_found) {
    /* Checking if memory limit was reached */
    if (*Usage == CAPACITY) {
        print_system_error(Error_73);
        *errors_found = 1;
        (*Usage)++;  /* Incrementing usage count so the next iteration will not print another error message */
        return;  /* Scanning line finished */
    }
    if (*Usage > CAPACITY) {  /* Checking if memory limit was exceeded */
        return;  /* Scanning line finished */
    }
    /* Adding the code to the code array */
    code[*IC] = word;

    (*IC)++;  /* Incrementing data count */
    *Usage += 1;  /* Incrementing usage count */
}

void process_operation_code(unsigned short *code, int *Usage, int *IC, Line *line, int method, char *operand, int operands_num, int *errors_found) {
    unsigned short word = 0, temp = 0;

    /* Handling the word */
    switch (method) {
        case IMMEDIATE:
            operand++;  /* Skipping the 'HASH' sign */
            word |= BIT_ABSOLUTE_FLAG;  /* Setting bit 2 for "Absolute" */
            temp |= (unsigned short)(atoi(operand) & MASK_12BIT);
            word |= temp << SHIFT_DST_OPERAND;  /* Setting bits 3-14 */
            add_instruction_code(code,Usage,IC,word,errors_found);  /* Adding machine code (second word) */
            return;  /* Scanning line finished */
        case DIRECT:
            if (add_label(operand,*IC,OPERAND,TBD) == NULL) {  /* Indicates memory allocation failed */
                fclose(line->file);
                free_line(line);
                free_labels();
                free_macros();
                free_all_memory();
                exit(1);  /* Exiting program */
            }
            word |= BIT_MASK_SIGNAL;  /* Setting bits 0 and 1 to signal to the "second pass" to update this label address */
            word |= line->line_num << BIT_MASK_SIGNAL;  /* Setting bits 3-14 to represent the line number for a potential error message in the "second pass"*/
            add_instruction_code(code,Usage,IC,word,errors_found);  /* Adding machine code (second word) */
            return;  /* Scanning line finished */
        case INDIRECT_REGISTER:
            operand++;  /* Skipping the 'ASTERISK' sign */
            word |= BIT_ABSOLUTE_FLAG;  /* Setting bit 2 for "Absolute" */
            word |= which_regis(operand) << (operands_num == 1 ? SHIFT_DST_REGISTER : SHIFT_SRC_REGISTER);  /* Setting bits 3-5 or 6-8 */
            add_instruction_code(code,Usage,IC,word,errors_found);  /* Adding machine code (second word) */
            return;  /* Scanning line finished */
        case DIRECT_REGISTER:
            word |= BIT_ABSOLUTE_FLAG;  /* Setting bit 2 for "Absolute" */
            word |= which_regis(operand) << (operands_num == 1 ? SHIFT_DST_REGISTER : SHIFT_SRC_REGISTER);  /* Setting bits 3-5 or 6-8 */
            add_instruction_code(code,Usage,IC,word,errors_found);  /* Adding machine code (second word) */
    }
}

void handle_one_operand(unsigned short *code, int *Usage, int *IC, Line *line, int method, char *operand, int ind, int *errors_found) {
    unsigned short word = 0;
    Op_Code *opcodes = get_opcodes();

    word |= (ind << SHIFT_OPCODE_POS) | BIT_ABSOLUTE_FLAG;

    word |= (BIT_MASK << (method + SHIFT_DST_OPERAND));  /* Setting the matching bit (3 to 6) */
    add_instruction_code(code,Usage,IC,word,errors_found);  /* Adding machine code (first word) */

    /* Handling the second word */
    process_operation_code(code,Usage,IC,line,method,operand,opcodes[ind].operands_num,errors_found);
}

void handle_two_operands(unsigned short *code, int *Usage, int *IC, Line *line, char *operand, char *second_operand, int ind, int *errors_found) {
    unsigned short word = 0, second_word = 0;
    Op_Code *opcodes = get_opcodes();
    int method = which_addressing_method(operand,line,errors_found);
    int method_2 = which_addressing_method(second_operand,line,errors_found);

    word |= (ind << SHIFT_OPCODE_POS) | BIT_ABSOLUTE_FLAG;
    word |= (BIT_MASK << (method_2 + SHIFT_DST_OPERAND));  /* Setting the matching bit (range 3-6) */
    word |= (BIT_MASK << (method + SHIFT_SRC_OPERAND));  /* Setting the matching bit (range 7-10) */
    add_instruction_code(code,Usage,IC,word,errors_found);  /* Adding machine code (first word) */

    /* Handling a special case for a shared word */
    if ((method == INDIRECT_REGISTER || method == DIRECT_REGISTER) &&
        (method_2 == INDIRECT_REGISTER || method_2 == DIRECT_REGISTER)) {
        second_word |= BIT_ABSOLUTE_FLAG;
        if (method == INDIRECT_REGISTER)
            operand++;  /* Skipping the 'ASTERISK' sign */
        if (method_2 == INDIRECT_REGISTER)
            second_operand++;  /* Skipping the 'ASTERISK' sign */
        second_word |= which_regis(operand) << SHIFT_SRC_REGISTER;  /* Setting bits 6-8 */
        second_word |= which_regis(second_operand) << SHIFT_DST_REGISTER;  /* Setting bits 3-5 */
        add_instruction_code(code,Usage,IC,second_word,errors_found);  /* Adding machine code (second word) */
        return;  /* Scanning line finished */
        }
    /* Handling the second word */
    process_operation_code(code,Usage,IC,line,method,operand,opcodes[ind].operands_num,errors_found);
    /* Handling the third word */
    process_operation_code(code,Usage,IC,line,method_2,second_operand,opcodes[ind].operands_num-1,errors_found);  /* operands_num-1 to signal that opernd is of type "destination" */
}