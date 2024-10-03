/**
 * This is the analyzer header file.
 */
#ifndef ANALYZER_H
#define ANALYZER_H
#include "utility.h"

typedef enum Addressing_Method {
    IMMEDIATE,
    DIRECT,
    INDIRECT_REGISTER,
    DIRECT_REGISTER
} Addressing_Method;

typedef enum valid_methods {
    ALL,
    NONE,
    METHOD_1,
    METHODS_1_2,
    METHODS_1_2_3
} valid_methods;

/* Operation struct definition */
typedef struct Op_Code {
    char *operation;
    int code_num;
    int operands_num;
    valid_methods source_methods;
    valid_methods destination_methods;
} Op_Code;

/**
 * Gets the opcode array.
 * @return pointer to the opcode array.
 */
Op_Code *get_opcodes();

/**
 * Validates the given macro name.
 * @param file_name The name of the file containing the macro.
 * @param macro_name The macro name to be validated.
 * @param line_count The line number where the macro is defined.
 * @return 0 if the macro name is valid, 1 if errors were detected.
 */
int valid_macro_name(char *file_name,char *macro_name,int line_count);


/**
 * Validates the given label name.
 * @param label_name The label name to be validated.
 * @param type The type of the label (REGULAR, ENTRY, EXTERN, OPERAND).
 * @param line The line struct containing file name and line number information.
 * @param errors_found Pointer to an integer to keep track of errors found.
 * @return 0 if the label name is valid, 1 if errors were detected.
 */
int valid_label_name(char *label_name,Type type,Line *line,int *errors_found);


/**
 * Determines if the given string is a valid opcode.
 * @param str The string to be checked.
 * @return The index of the opcode if valid, -1 if not valid.
 */
int which_opcode(char *str);


/**
 * Determines if the given string is a valid register.
 * @param str The string to be checked.
 * @return The index of the register if valid, -1 if not valid.
 */
int which_regis(char *str);


/**
 * Determines if the given string is a valid instruction.
 * @param str The string to be checked.
 * @return The index of the instruction if valid, -1 if not valid.
 */
int which_instr(char *str);


/**
 * Determines the addressing method of the given operand.
 * @param opernad The operand to be checked.
 * @param line The line struct containing file name and line number information.
 * @param errors_found Pointer to an integer to keep track of errors found.
 * @return The addressing method if valid, -1 if not valid.
 */
int which_addressing_method(char *opernad,Line *line,int *errors_found);


/**
 * Determines if the given string is a reserved word.
 * @param file_name The name of the file containing the word.
 * @param str The word to be checked.
 * @param line_count The line number where the word is found.
 * @param type The type of the label (REGULAR, ENTRY, EXTERN, OPERAND).
 * @return 0 if the word is not reserved, 1 if it is reserved.
 */
int is_reserved_word(char *file_name,char *str,int line_count,Type type);


/**
 * Checks if the given line contains an instruction.
 * @param data The data array to store the machine code.
 * @param Usage Pointer to the usage counter.
 * @param DC Pointer to the data counter.
 * @param line The line struct containing file name and line number information.
 * @param ptr The current position in the line.
 * @param current_word The current word being processed.
 * @param errors_found Pointer to an integer to keep track of errors found.
 * @return 1 if the line contains an instruction, 0 if not.
 */
int is_instruction(unsigned short *data,int *Usage,int *DC,Line *line,char *ptr,char *current_word,int *errors_found);


/**
 * Checks if the given line contains an operation.
 * @param code The code array to store the machine code.
 * @param Usage Pointer to the usage counter.
 * @param IC Pointer to the instruction counter.
 * @param line The line struct containing file name and line number information.
 * @param ptr The current position in the line.
 * @param current_word The current word being processed.
 * @param errors_found Pointer to an integer to keep track of errors found.
 * @return 1 if the line contains an operation, 0 if not.
 */
int is_operation(unsigned short *code,int *Usage,int *IC,Line *line,char *ptr,char *current_word,int *errors_found);


/**
 * Checks if the given addressing method is legal.
 * @param line The line struct containing file name and line number information.
 * @param method The addressing method to be checked.
 * @param ind The index of the operation.
 * @param operands_num The number of operands for the operation.
 * @param errors_found Pointer to an integer to keep track of errors found.
 * @return 0 if the method is legal, 1 if not.
 */
int is_method_legal(Line *line,int method,int ind,int operands_num,int *errors_found);


/**
 * Handles the data found in the given line.
 * @param data The data array to store the machine code.
 * @param Usage Pointer to the usage counter.
 * @param DC Pointer to the data counter.
 * @param line The line struct containing file name and line number information.
 * @param ptr The current position in the line.
 * @param errors_found Pointer to an integer to keep track of errors found.
 */
void data_found(unsigned short *data,int *Usage,int *DC,Line *line,char *ptr,int *errors_found);


/**
 * Handles the string found in the given line.
 * @param data The data array to store the machine code.
 * @param Usage Pointer to the usage counter.
 * @param DC Pointer to the data counter.
 * @param line The line struct containing file name and line number information.
 * @param ptr The current position in the line.
 * @param errors_found Pointer to an integer to keep track of errors found.
 */
void string_found(unsigned short *data,int *Usage,int *DC,Line *line,char *ptr,int *errors_found);


/**
 * Handles the entry found in the given line.
 * @param line The line struct containing file name and line number information.
 * @param ptr The current position in the line.
 * @param errors_found Pointer to an integer to keep track of errors found.
 */
void entry_found(Line *line,char *ptr,int *errors_found);


/**
 * Handles the extern found in the given line.
 * @param line The line struct containing file name and line number information.
 * @param ptr The current position in the line.
 * @param errors_found Pointer to an integer to keep track of errors found.
 */
void extern_found(Line *line,char *ptr,int *errors_found);


/**
 * Validates the operation found in the given line.
 * @param code The code array to store the machine code.
 * @param Usage Pointer to the usage counter.
 * @param IC Pointer to the instruction counter.
 * @param line The line struct containing file name and line number information.
 * @param ptr The current position in the line.
 * @param ind The index of the operation.
 * @param errors_found Pointer to an integer to keep track of errors found.
 */
void valid_operation(unsigned short *code,int *Usage,int *IC,Line *line,char *ptr,int ind,int *errors_found);


/**
 * Analyzes the numbers in the ".data" instruction line and encodes them into machine code.
 * @param data Pointer to the array holding the machine code for data.
 * @param Usage Pointer to the usage counter for memory.
 * @param DC Pointer to the data counter.
 * @param line Pointer to the structure representing the current line of code being processed.
 * @param ptr The current position in the line.
 * @param errors_found Pointer to the error counter.
 */
void analyze_numbers(unsigned short *data,int *Usage,int *DC,Line *line,char *ptr,int *errors_found);


#endif