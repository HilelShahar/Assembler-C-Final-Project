/**
 * This is the machine code header file.
 */
#ifndef MACHINE_CODE_H
#define MACHINE_CODE_H
#include "utility.h"

/**
 * Adds a data code to the data array.
 * Converts the given number to its 15-bit 2's complement binary representation and adds it to the array.
 * @param data Pointer to the array holding the data code.
 * @param DC Pointer to the data counter.
 * @param number The number to be added as data code.
 */
void add_data_code(unsigned short *data,int *DC,int number);


/**
 * Adds an instruction code to the code array.
 * Checks for memory limits before adding the code to the array.
 * @param code Pointer to the array holding the instruction code.
 * @param Usage Pointer to the usage counter for memory.
 * @param IC Pointer to the instruction counter.
 * @param word The instruction code to be added.
 * @param errors_found Pointer to the error counter.
 */
void add_instruction_code(unsigned short *code,int *Usage,int *IC,unsigned short word,int *errors_found);


/**
 * Processes and encodes the operation code for an operand.
 * Handles different addressing methods and updates the machine code accordingly.
 * @param code Pointer to the array holding the machine code.
 * @param Usage Pointer to the usage counter for memory.
 * @param IC Pointer to the instruction counter.
 * @param line Pointer to the structure representing the current line of code being processed.
 * @param method The addressing method of the operand.
 * @param operand The operand string.
 * @param operands_num Number of operands in the instruction.
 * @param errors_found Pointer to the error counter.
 */
void process_operation_code(unsigned short *code,int *Usage,int *IC,Line *line,int method,char *operand,int operands_num,int *errors_found);


/**
 * Handles the encoding and processing of an instruction with one operand.
 * Generates machine code for the instruction and manages the different addressing methods for the operand.
 * @param code Pointer to the array holding the machine code.
 * @param Usage Pointer to the usage counter for memory.
 * @param IC Pointer to the instruction counter.
 * @param line Pointer to the structure representing the current line of code being processed.
 * @param method The addressing method of the operand.
 * @param operand The operand string.
 * @param ind Index of the instruction in the opcode table.
 * @param errors_found Pointer to the error counter.
 */
void handle_one_operand(unsigned short *code,int *Usage,int *IC,Line *line,int method,char *operand,int ind,int *errors_found);


/**
 * Handles the encoding and processing of an instruction with two operands.
 * Generates machine code for the instruction and manages the different addressing methods for the operands.
 * @param code Pointer to the array holding the machine code.
 * @param Usage Pointer to the usage counter for memory.
 * @param IC Pointer to the instruction counter.
 * @param line Pointer to the structure representing the current line of code being processed.
 * @param operand The first operand string.
 * @param second_operand The second operand string.
 * @param ind Index of the instruction in the opcode table.
 * @param errors_found Pointer to the error counter.
 */
void handle_two_operands(unsigned short *code,int *Usage,int *IC,Line *line,char *operand,char *second_operand,int ind,int *errors_found);


#endif