/**
 * This is the first pass header file.
 * Explanation of the process:
 * When a label declaration is encountered, it validates the label and stores it with its corresponding
 * address, type, and location information. For lines that contain operations or instructions,
 * the function proceeds to scan for operands, validate them, and then convert the operation into machine code.
 * If an operand is a label, the function sets bits 0 and 1 in the instruction array to indicate to the second pass
 * that this label's address has not yet been coded and will need to be updated during the second pass.
 * This ensures that all label references are correctly handled later in the assembly process.
 */
#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include "utility.h"


/**
 * This function performs the first pass of the assembler.
 * It scans the file to identify and store instructions, operations, and labels,
 * converting them into machine code while handling potential errors.
 * If no errors were detected, it calls the second pass.
 * @param file_name The name of the file to process.
 * @return 0 if successful, 1 if errors were detected.
 */
int first_pass(char *file_name);


/**
 * Scans the text of the provided file, processing each line to identify and
 * handle instructions, operations, and labels, converting them into machine code.
 * @param file_am_name The name of the file to scan.
 * @param code Array to store the instruction code.
 * @param data Array to store the data code.
 * @param IC Instruction Counter.
 * @param DC Data Counter.
 * @return 0 if no errors were detected, 1 otherwise.
 */
int scan_text(char *file_am_name,unsigned short *code,unsigned short *data,int *IC,int *DC);


/**
 * Processes each word in a line to identify and handle instructions,
 * operations, and labels, before converting them into machine code.
 * @param code Array to store the instruction code.
 * @param data Array to store the data code.
 * @param Usage Counter for usage of memory.
 * @param IC Instruction Counter.
 * @param DC Data Counter.
 * @param line The current line being processed.
 * @param errors_found Number stored to indicate success/faliure.
 */
void scan_word(unsigned short *code,unsigned short *data,int *Usage,int *IC,int *DC,Line *line,int *errors_found);


#endif