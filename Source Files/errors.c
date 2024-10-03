/**
 * This file handles the printing of errors.
 */
#include "../Header Files/errors.h"
#include <stdio.h>

/* The Following array specifies the error code numbers and the corresponding error message */
Error errors[] = {
        {Error_0, " [CODE_0] | ERROR | No files entered"},
        {Error_1, " [CODE_1] | ERROR | Memory allocation failed"},
        {Error_2, " [CODE_2] | ERROR | File names should be entered without \".as\" extention"},
        {Error_3, "Illegal comment, whitespace characters are not allowed before the semicolon (;)"},
        {Error_4, " [CODE_4] | ERROR | Failed to delete redundant file"},
        {Error_5, " [CODE_5] | ERROR | Failed to open existing file for reading"},
        {Error_6, " [CODE_6] | ERROR | Failed to open new file for writing"},
        {Error_7, "The line is too long, 80 characters max"},
        {Error_8, "Invalid macro declaration, no name was defined"},
        {Error_9, "Invalid macro declaration, \"macr\" command with extraneous text"},
        {Error_10, "Invalid macro declaration, an opcode name cannot be used as a macro name"},
        {Error_11, "Invalid macro declaration, a register name cannot be used as a macro name"},
        {Error_12, "Invalid macro declaration, an instruction name cannot be used as a macro name"},
        {Error_13, "Invalid macro declaration, macro should be assigned with one name"},
        {Error_14, "Invalid macro declaration, this macro name is already in use"},
        {Error_15, "Invalid macro declaration, \"endmacr\" command with extraneous text"},
        {Error_16, "Invalid macro declaration, macro name is too long, 31 characters max"},
        {Error_17, "Invalid macro declaration, empty macro content"},
        {Error_18, "Invalid macro declaration, \"macr\" is a reserved word"},
        {Error_19, "Invalid macro declaration, \"endmacr\" is a reserved word"},
        {Error_20, "Invalid label declaration, label name is too long, 31 characters max"},
        {Error_21, "Invalid label declaration, label name must contain alphabetic or numeric characters only"},
        {Error_22, "Invalid label declaration, no value associated with label"},
        {Error_23, "Invalid label declaration, a label name cannot be the same as a macro name"},
        {Error_24, "Invalid label declaration, an opcode name cannot be used as a label name"},
        {Error_25, "Invalid label declaration, a register name cannot be used as a label name"},
        {Error_26, "Invalid label declaration, an instruction name cannot be used as a label name"},
        {Error_27, "Invalid label declaration, this label name is already in use"},
        {Error_28, "Invalid label declaration, no name was defined"},
        {Error_29, "Invalid label declaration, label name must start with an alphabetical character"},
        {Error_30, "Invalid label declaration, \"macr\" is a reserved word"},
        {Error_31, "Invalid label declaration, \"endmacr\" is a reserved word"},
        {Error_32, "Invalid label declaration, local label name cannot be the same as an external label name"},  /* "Local" means regular/entry label */
        {Error_33, "This macro call was missed in pre-processing, macros calls are allowed once per line and with no extra text"},
        {Error_34, "Instruction \".data\" has an illegal comma following the instruction name"},
        {Error_35, "Instruction \".data\" has illegal characters, only integers allowed"},
        {Error_36, "Instruction \".data\" has a missing comma"},
        {Error_37, "Instruction \".data\" has multiple consecutive commas"},
        {Error_38, "Instruction \".data\" expects an integer after the last comma"},
        {Error_39, "Instruction \".data\" has a number that is out of range"},
        {Error_40, "Instruction \".data\" has no parameters"},
        {Error_41, "Instruction \".string\" expects one string parameter enclosed in double quotes (\" \")"},
        {Error_42, "Instruction \".string\" has no parameter"},
        {Error_43, "Instructions \".entry\" and \".extern\" must have a label declaration"},
        {Error_44, "Instructions \".entry\" and \".extern\" allow one label declaration at a time"},
        {Error_45, "Instructions \".entry\" and \".extern\" label name must refer to a valid label"},
        {Error_46, "Instructions \".entry\" and \".extern\" label name cannot be the same as a macro name"},
        {Error_47, "Instruction \".entry\" label name cannot be the same as an external label name"},
        {Error_48, "Instruction \".extern\" label name cannot be the same as a local label name"},  /* "Local" means regular/entry label */
        {Error_49, "This operation has extraneous text, no operands required"},
        {Error_50, "This operation has a missing operand"},
        {Error_51, "This operation has missing operands"},
        {Error_52, "This operation has extraneous text, only one operand is required"},
        {Error_53, "This operation has an illegal comma after the operation name"},
        {Error_54, "This operation has extraneous text, only two operands are required"},
        {Error_55, "This operation has multiple consecutive commas"},
        {Error_56, "This operation has a missing comma"},
        {Error_57, "This operation uses an illegal method for a destination operand"},
        {Error_58, "This operation uses an illegal method for a source operand"},
        {Error_59, "This operand was recognized as a label but is not valid, please check syntax"},
        {Error_60, "This operation has an operand that uses an 'IMMEDIATE' method type but has no value"},
        {Error_61, "This opernad is invalid for an 'IMMEDIATE' method type, only integers allowed"},
        {Error_62, "This opernad is out of range for an 'IMMEDIATE' method type"},
        {Error_63, "This operation has an opernad that uses an 'INDIRECT_REGISTER' method type but has no value"},
        {Error_64, "This opernad is not a valid register for an 'INDIRECT_REGISTER' method type"},
        {Error_65, "Unrecognized command, note that label declarations must have a space after the colon (:)"},  /* Example: "LABEL:mov..." */
        {Error_66, "Unrecognized command, note that label declarations must have the colon (:) attached to the label name"},  /* Example: "LABEL : mov..." */
        {Error_67, "Unrecognized command, note that an instruction must start with a dot (.)"},  /* Example: "entry LABEL" */
        {Error_68, "Unrecognized command, please check syntax"},
        {Error_69, "Unrecognized operand, please check syntax"},
        {Error_70, "Invalid operand, note that reserved words and macro names cannot be used as operands"},
        {Error_71, "Label name is not a valid command"},  /* Example: "LABEL r7" */
        {Error_72, " was declared as \".entry\" but was not defined"},
        {Error_73, " [CODE_8] | ERROR | Memory capacity exceeded! Assembler machine-coding is suspended, however line scanning continues"}
};

void print_system_error(int error_code) {
        /* Printing the error message */
        printf("%s\n", errors[error_code].error_msg);
}

void print_syntax_error(int error_code, char *file_name, int line_num) {
    /* Printing the file name, assembly line number and the error message */
    printf(" ERROR | File \"%s\" at line %d | %s\n",file_name,line_num,errors[error_code].error_msg);
}

void print_specific_error(int error_code, char *file_name, int line_num, char *word) {
    /* Printing the file name, assembly line number, specific error quoted and the error message */
    printf(" ERROR | File \"%s\" at line %d | \"%s\" | %s\n",file_name,line_num,word,errors[error_code].error_msg);
}