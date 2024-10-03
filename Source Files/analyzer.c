/**
 * This is the largest file in the project, it handles the entire analysis of the assembly code.
 * It includes functions for validating labels, registers, operations and instructions,
 * as well as determining their types while handling all types of potential errors.
 * This phase ensures that the input assembly code is correctly formatted and valid
 * before proceeding to the code generation phase.
 * The file also contains definitions for opcodes, registers, and instructions.
 */
#include "../Header Files/analyzer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header Files/definitions.h"
#include "../Header Files/errors.h"
#include "../Header Files/labels.h"
#include "../Header Files/machine_code.h"
#include "../Header Files/macros.h"
#include "../Header Files/utility.h"

/* Defining the opcodes */
Op_Code OPCODES[] = {
    {"mov", 0, 2, ALL, METHODS_1_2_3},
    {"cmp", 1, 2, ALL, ALL},
    {"add", 2, 2, ALL, METHODS_1_2_3},
    {"sub", 3, 2, ALL, METHODS_1_2_3},
    {"lea", 4, 2, METHOD_1, METHODS_1_2_3},
    {"clr", 5, 1, NONE, METHODS_1_2_3},
    {"not", 6, 1, NONE, METHODS_1_2_3},
    {"inc", 7, 1, NONE, METHODS_1_2_3},
    {"dec", 8, 1, NONE, METHODS_1_2_3},
    {"jmp", 9, 1, NONE, METHODS_1_2},
    {"bne", 10, 1, NONE, METHODS_1_2},
    {"red", 11, 1, NONE, METHODS_1_2_3},
    {"prn", 12, 1, NONE, ALL},
    {"jsr", 13, 1, NONE, METHODS_1_2},
    {"rts", 14, 0, NONE, NONE},
    {"stop", 15, 0, NONE, NONE}
};

/* Defining the registers */
char *REGISTERS[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/* Defining the instructions */
char *INSTRUCTIONS[] = {".data", ".string", ".entry", ".extern"};

Op_Code *get_opcodes() {
    return OPCODES;
}

int valid_macro_name(char *file_name, char *macro_name, int line_count) {
    /* Checking if there is more than one name */
    if (contains_whitespace(macro_name)) {
        print_syntax_error(Error_13,file_name,line_count);
        return 1;  /* Indicates faliure */
    }
    /* Checking if macro length is valid */
    if (strlen(macro_name) > MAX_MACRO_LENGTH) {
        print_syntax_error(Error_16,file_name,line_count);
        return 1;  /* Indicates failure */
    }
    /* Comparing the macro name with each of the system's reserved words */
    if (is_reserved_word(file_name,macro_name,line_count,REGULAR) != 0)
        return 1;  /* Indicates failure */

    return 0;  /* Indicates success */
}

int valid_label_name(char *label_name, Type type, Line *line, int *errors_found) {
    char first_char;
    int i, label_name_len;
    Label *label;
    label_name_len = strlen(label_name);
    first_char = label_name[0];


    /* Checking if the current line is not an entry/extern instruction line */
    if (type == REGULAR) {
        label_name[label_name_len-1] = NULL_TERMINATOR;  /* Getting the label name without ':' */
        label_name_len -= 1;
    }
    /* Checking if the name is empty */
    if (*label_name == NULL_TERMINATOR) {
        print_syntax_error(type == REGULAR ? Error_28 : type == OPERAND ? Error_59 : Error_43,
            line->file_am_name,line->line_num);
        *errors_found = 1;
        return 1;  /* Indicates label name is not valid */
    }
    /* Checking if the first character is an alphabetic */
    if (!isalpha(first_char)) {
        if (which_instr(label_name) != -1 && type == REGULAR) {  /* Checking if the label name is an instruction in case of a non alphabetic first character */
            print_syntax_error(Error_26,line->file_am_name,line->line_num);
            *errors_found = 1;
            return 1;  /* Indicates label name is not valid */
        }
        print_specific_error(type == REGULAR ? Error_29 : type == OPERAND ? Error_59 : Error_45,
            line->file_am_name,line->line_num,label_name);
        *errors_found = 1;
        return 1;  /* Indicates label name is not valid */
    }
    /* Checking if the label name length is valid */
    if (label_name_len > MAX_LABEL_LENGTH) {
        print_specific_error(type == REGULAR ? Error_20 : type == OPERAND ? Error_59 : Error_45,
            line->file_am_name,line->line_num,label_name);
        *errors_found = 1;
        return 1;  /* Indicates label name is not valid */
    }
    /* Checking if the label name only contains alphabetic or numeric characters */
    for (i=0; label_name[i] != NULL_TERMINATOR; i++) {
        if (!isalnum(label_name[i])) {
            print_specific_error(type == REGULAR ? Error_21 : type == OPERAND ? Error_59 : Error_45,
                line->file_am_name,line->line_num,label_name);
            *errors_found = 1;
            return 1;  /* Indicates label name is not valid */
        }
    }
    /* Checking if the label name is a macro name */
    if (is_macro_name(label_name) != NULL) {
        print_specific_error(type == REGULAR ? Error_23 : type == OPERAND ? Error_70 : Error_46,
            line->file_am_name,line->line_num,label_name);
        *errors_found = 1;
        return 1;  /* Indicates label name is not valid */
    }
    /* Checking if the label name is a reserved word */
    if (is_reserved_word(line->file_am_name,label_name,line->line_num,type) != 0) {
        *errors_found = 1;
        return 1;  /* Indicates label name is not valid */
    }
    if (type == OPERAND)
        return 0;  /* Finished validation for type "operand" */

    /* Checking if the label name had already been defined */
    label = is_label_name(label_name);
    if (label != NULL) {
        if (type == ENTRY) {
            if (label->type == EXTERN) {
                print_syntax_error(Error_47,line->file_am_name,line->line_num);
                *errors_found = 1;
                return 1;  /* Indicates label name is not valid */
            }
            if (label->type == ENTRY) {
                printf(" WARNING | File \"%s\" at line %d | Instructions \".entry\" or"
                " \".extern\" duplicate declarations will be ignored\n",
                line->file_am_name,line->line_num);
                return 1;  /* Scanning line finished */
            }
            label->type = ENTRY;  /* Updating label type */
            return 1;  /* Scanning line finished */
        }
        if (type == EXTERN) {
            if (label->type != EXTERN) {
                print_syntax_error(Error_48,line->file_am_name,line->line_num);
                *errors_found = 1;
                return 1;  /* Indicates label name is not valid */
            }
            printf(" WARNING | File \"%s\" at line %d | Instructions \".entry\" or"
            " \".extern\" duplicate declarations will be ignored\n",
            line->file_am_name,line->line_num);
            return 1;  /* Scanning line finished */
        }
        /* If this line was reached then label type is "REGULAR" */
        if (label->type == EXTERN) {
            print_specific_error(Error_32,line->file_am_name,line->line_num,label_name);
            *errors_found = 1;
            return 1;  /* Indicates label name is not valid */
        }
        if (label->type == REGULAR) {
            print_specific_error(Error_27,line->file_am_name,line->line_num,label_name);
            *errors_found = 1;
            return 1;  /* Indicates label name is not valid */
        }
        if (label->type == ENTRY && label->location != TBD) {
            print_specific_error(Error_27,line->file_am_name,line->line_num,label_name);
            *errors_found = 1;
            return 1;  /* Indicates label name is not valid */
        }
        remove_label(label);
        return -1;  /* Special case - signaling to create a new label of type "entry" */
    }
    return 0;  /* Indicates label name is valid */
}

int which_opcode(char *str) {
    int i;

    if (str == NULL)  /* Indicates string is not an opcode name */
        return -1;

    /* Comparing the string with each of the system opcodes */
    for (i = 0; i < OPCODES_COUNT; i++) {
        if (strcmp(str,OPCODES[i].operation) == 0) {
            return i;  /* Returning the index of the matching opcode */
        }
    }
    return -1;  /* Indicates string is not an opcode name */
}

int which_regis(char *str) {
    int i;

    if (str == NULL)  /* Indicates string is not a register name */
        return -1;

    /* Comparing the string with each of the system registers */
    for (i = 0; i < REGISTERS_COUNT; i++) {
        if (strcmp(str, REGISTERS[i]) == 0) {
            return i; /* Returning the index of the matching register */
        }
    }
    return -1;  /* Indicates string is not a register name */
}

int which_instr(char *str) {
    int i;

    if (str == NULL)  /* Indicates string is not an instruction name */
        return -1;

    /* Comparing the string with each of the system instructions */
    for (i = 0; i < INSTRUCTIONS_COUNT; i++) {
        if (strcmp(str, INSTRUCTIONS[i]) == 0) {
            return i;  /* Returning the index of the matching instruction */
        }
    }
    return -1;  /* Indicates string is not an instruction name */
}

int which_addressing_method(char *operand, Line *line, int *errors_found) {
    char *endptr;
    long val;

    if (operand[0] == HASH) {
        operand++;
        /* Checking if no value was entered */
        if (*operand == NULL_TERMINATOR) {
            print_syntax_error(Error_60,line->file_am_name,line->line_num);
            *errors_found = 1;
            return -1;  /* Indicates faliure */
        }
        val = strtol(operand,&endptr,DECIMAL_BASE);
        /* Checking if the conversion was successful */
        if (*endptr != NULL_TERMINATOR || endptr == operand) {
            print_specific_error(Error_61,line->file_am_name,line->line_num,operand);
            *errors_found = 1;
            return -1;  /* Indicates faliure */
        }
        /* Checking if the number is in range */
        if (val < MIN_12BIT || val > MAX_12BIT) {
            print_specific_error(Error_62,line->file_am_name,line->line_num,operand);
            *errors_found = 1;
            return -1;  /* Indicates faliure */
        }
        return IMMEDIATE;  /* Returning the matching addressing method */
    }
    if (operand[0] == ASTERISK) {
        operand++;
        /* Checking if no value was entered */
        if (*operand == NULL_TERMINATOR) {
            print_syntax_error(Error_63,line->file_am_name,line->line_num);
            *errors_found = 1;
            return -1;  /* Indicates faliure */
        }
        if (which_regis(operand) == -1) {
            print_specific_error(Error_64,line->file_am_name,line->line_num,operand);
            *errors_found = 1;
            return -1;  /* Indicates faliure */
        }
        return INDIRECT_REGISTER;  /* Returning the matching addressing method */
    }
    if (which_regis(operand) != -1)  /* Indicates opernad is a register */
        return DIRECT_REGISTER;  /* Returning the matching addressing method */

    /* If this line was reached then the opernad is a potential label - validating */
    if (valid_label_name(operand,OPERAND,line,errors_found) != 0)
        return -1;  /* Indicates faliure */

    /* If the label is valid - returning the matching addressing method */
    return DIRECT;
}

int is_reserved_word(char *file_name, char *str, int line_count, Type type) {
    int len = strlen(file_name);

    /* Comparing the string with each of the system's reserved words and determining the error message */
    if (which_opcode(str) != -1) {  /* Function returns the index of the matching word or -1 if no word matched */
        if (strcmp(&file_name[len-EXTENSION_LEN],".as") == 0 && type != OPERAND) {  /* Indicates this is a macro name validation */
            print_syntax_error(Error_10,file_name,line_count);
            return 1;  /* Indicates the name is invalid */
        }
        print_syntax_error(type == OPERAND ? Error_70 : type == REGULAR ? Error_24 : Error_45,
            file_name,line_count);
        return 1;  /* Indicates the name is invalid */
    }
    if (which_regis(str) != -1) {
        if (strcmp(&file_name[len-EXTENSION_LEN],".as") == 0 && type != OPERAND) {  /* Indicates this is a macro name validation */
            print_syntax_error(Error_11,file_name,line_count);
            return 1;  /* Indicates the name is invalid */
        }
        print_syntax_error(type == OPERAND ? Error_70 : type == REGULAR ? Error_25 : Error_45,
            file_name,line_count);
        return 1;  /* Indicates the name is invalid */
    }
    if (which_instr(str) != -1) {
        if (strcmp(&file_name[len-EXTENSION_LEN],".as") == 0 && type != OPERAND) {  /* Indicates this is a macro name validation */
            print_syntax_error(Error_12,file_name,line_count);
            return 1;  /* Indicates the name is invalid */
        }
        print_syntax_error(type == OPERAND ? Error_70 : type == REGULAR ? Error_26 : Error_45,
            file_name,line_count);
        return 1;  /* Indicates the name is invalid */
    }
    if (strcmp(str,"macr") == 0) {
        if (strcmp(&file_name[len-EXTENSION_LEN],".as") == 0 && type != OPERAND) {  /* Indicates this is a macro name validation */
            print_syntax_error(Error_18,file_name,line_count);
            return 1;  /* Indicates the name is invalid */
        }
        print_syntax_error(type == OPERAND ? Error_70 : type == REGULAR ? Error_30 : Error_45,
            file_name,line_count);
        return 1;  /* Indicates the name is invalid */
    }
    if (strcmp(str,"endmacr") == 0) {
        if (strcmp(&file_name[len-EXTENSION_LEN],".as") == 0 && type != OPERAND) {  /* Indicates this is a macro name validation */
            print_syntax_error(Error_19,file_name,line_count);
            return 1;  /* Indicates the name is invalid */
        }
        print_syntax_error(type == OPERAND ? Error_70 : type == REGULAR ? Error_31 : Error_45,
            file_name,line_count);
        return 1;  /* Indicates the name is invalid */
    }
    return 0;  /* Indicates the name is valid */
}

int is_instruction(unsigned short *data, int *Usage, int *DC, Line *line, char *ptr, char *current_word, int *errors_found) {
    int curr_word_len = strlen(current_word);

    /* Checking for a potential instruction */
    switch (which_instr(current_word)) {
        case 0:
            ptr += curr_word_len;  /* Skipping the first word ".data" */
            data_found(data,Usage,DC,line,ptr,errors_found);
            return 1;  /* Scanning line finished */
        case 1:
            ptr += curr_word_len;  /* Skipping the first word ".string" */
            string_found(data,Usage,DC,line,ptr,errors_found);
            return 1;  /* Scanning line finished */
        case 2:
            ptr += curr_word_len;  /* Skipping the first word ".entry" */
            entry_found(line,ptr,errors_found);
            return 1;  /* Scanning line finished */
        case 3:
            ptr += curr_word_len;  /* Skipping the first word ".extern" */
            extern_found(line,ptr,errors_found);
            return 1;  /* Scanning line finished */
        default:
            return 0;  /* Indicates line is not an "instruction" line, continue scanning */
    }
}

int is_operation(unsigned short *code, int *Usage, int *IC, Line *line, char *ptr, char *current_word, int *errors_found) {
    int curr_word_len = strlen(current_word), ind;

    /* Checking for a potential operation */
    ind = which_opcode(current_word);
    if (ind != -1) {  /* Indicates line is an "operation" line */
        /* Updating label properties */
        if (line->label != NULL) {
            line->label->address = *IC + STARTING_ADDRESS;
            line->label->location = CODE;
        }
        ptr += curr_word_len;  /* Skipping the first word */

        valid_operation(code,Usage,IC,line,ptr,ind,errors_found);  /* Validating operation */
        return 1;  /* Scanning line finished */
    }
    return 0;  /* Indicates line is not an "operation" line, continue scanning */
}

int is_method_legal(Line *line, int method, int ind, int operands_num, int *errors_found) {
    /* Getting the legal methods for the current operation */
    if (operands_num == 1) {  /* Indicates operand is of type "destination" */
        switch (OPCODES[ind].destination_methods) {
            case METHODS_1_2:
                if (method == IMMEDIATE || method == DIRECT_REGISTER) {
                    print_syntax_error(Error_57,line->file_am_name,line->line_num);
                    *errors_found = 1;
                    return 1;  /* Indicates method is illegal */
                }
                return 0;  /* Indicates method is legal */
            case METHODS_1_2_3:
                if (method == IMMEDIATE) {
                    print_syntax_error(Error_57,line->file_am_name,line->line_num);
                    *errors_found = 1;
                    return 1;  /* Indicates method is illegal */
                }
            default:
                return 0;  /* Indicates method is legal */
        }
    }
    /* If this line was reached then operand is of type "source" */
    if (OPCODES[ind].source_methods == METHOD_1) {
        if (method != DIRECT) {
            print_syntax_error(Error_58,line->file_am_name,line->line_num);
            *errors_found = 1;
            return 1;  /* Indicates method is illegal */
        }
    }
    return 0;  /* Indicates method is legal */
}

void data_found(unsigned short *data, int *Usage, int *DC, Line *line, char *ptr, int *errors_found) {
    /* Checking if there are no parameters */
    if (*ptr == NULL_TERMINATOR) {
        if (line->label != NULL)
            remove_last_label();
        print_syntax_error(Error_40,line->file_am_name,line->line_num);
        *errors_found = 1;
        return;
    }
    /* Analyzing input numbers */
    analyze_numbers(data,Usage,DC,line,ptr,errors_found);
}

void string_found(unsigned short *data, int *Usage, int *DC, Line *line, char *ptr, int *errors_found) {
    char *trimmed_line;
    int trimmed_line_len, i;

    /* Checking for a parameter */
    if (*ptr == NULL_TERMINATOR) {
        if (line->label != NULL)
            remove_last_label();
        print_syntax_error(Error_42,line->file_am_name,line->line_num);
        *errors_found = 1;
        return;
    }
    trimmed_line = trim_whitespace(ptr);
    trimmed_line_len = strlen(trimmed_line);

    /* Checking for double quotes at the start and the end of the string */
    if (trimmed_line[0] != DOUBLE_QUOTE || trimmed_line[trimmed_line_len-1] != DOUBLE_QUOTE) {
        if (line->label != NULL)
            remove_last_label();
        print_syntax_error(Error_41,line->file_am_name,line->line_num);
        *errors_found = 1;
        return;
    }
    /* Checking if the string is empty */
    if (strlen(trimmed_line) == TWO) {   /* Indicates string contains only double quotes */
        printf(" WARNING | File \"%s\" at line %d | Instruction \".string\" parameter"
               " is an empty string\n",line->file_am_name,line->line_num);
    }
    trimmed_line[trimmed_line_len-1] = NULL_TERMINATOR;
    trimmed_line++;
    trimmed_line_len = strlen(trimmed_line);

    /* Updating label properties */
    if (line->label != NULL) {
        line->label->address = *DC;
        line->label->location = DATA;
    }
    /* Adding machine code to data array */
    for (i=0; i<trimmed_line_len; i++) {
        if (*Usage + 1 == CAPACITY) {  /* Checking if memory limit was reached (+1 to account for the null-terminator) */
            print_system_error(Error_73);
            *errors_found = 1;
            (*Usage)++;  /* Incrementing usage count so the next iteration will not print another error message */
            return;  /* Scanning line finished */
        }
        if (*Usage + 1 > CAPACITY) {  /* Checking if memory limit was exceeded */
            return;  /* Scanning line finished */
        }
        /* Getting the ASCII value by converting 'char' type to 'int' and then adding code */
        add_data_code(data,DC,(int)trimmed_line[i]);
        *Usage += 1;  /* Incrementing usage count */
    }
    add_data_code(data,DC,0);  /* Adding the null-terminator */
    *Usage += 1;  /* Incrementing usage count */
}

void entry_found(Line *line, char *ptr, int *errors_found) {
    Label *label;
    char *trimmed_line;

    /* Checking if a label was already declared at the current line */
    if (line->label != NULL) {
        printf(" WARNING | File \"%s\" at line %d | Label defined at the start of an"
        " \".entry\" or \".extern\" instruction line will be ignored\n",line->file_am_name,line->line_num);
        remove_label(line->label);
    }
    /* Checking if there is no label declaration */
    if (*ptr == NULL_TERMINATOR) {
        print_syntax_error(Error_43,line->file_am_name,line->line_num);
        *errors_found = 1;
        return;  /* Scanning line finished */
    }
    /* Checking if there is more than one label declaration */
    trimmed_line = trim_whitespace(ptr);
    if (contains_whitespace(trimmed_line)) {
        print_syntax_error(Error_44,line->file_am_name,line->line_num);
        *errors_found = 1;
        return;  /* Scanning line finished */
    }
    if (valid_label_name(trimmed_line,ENTRY,line,errors_found) != 0)
        return;  /* Scanning line finished */

    label = add_label(trimmed_line,0,ENTRY,TBD);
    if (label == NULL) {  /* Indicates memory allocation failed */
        fclose(line->file);
        free_line(line);
        free_labels();
        free_macros();
        free_all_memory();
        exit(1);  /* Exiting program */
    }
    line->label = label;  /* Setting the label pointer of struct line to the new entry label */
}

void extern_found(Line *line, char *ptr, int *errors_found) {
    Label *label;
    char *trimmed_line;

    /* Checking if a label was already declared at the current line */
    if (line->label != NULL) {
        printf(" WARNING | File \"%s\" at line %d | Label defined at the start of an"
        " \".entry\" or \".extern\" instruction line will be ignored\n",line->file_am_name,line->line_num);
        remove_label(line->label);
    }
    /* Checking if there is no label declaration */
    if (*ptr == NULL_TERMINATOR) {
        print_syntax_error(Error_43,line->file_am_name,line->line_num);
        *errors_found = 1;
        return;  /* Scanning line finished */
    }
    /* Checking if there is more than one label declaration */
    trimmed_line = trim_whitespace(ptr);
    if (contains_whitespace(trimmed_line)) {
        print_syntax_error(Error_44,line->file_am_name,line->line_num);
        *errors_found = 1;
        return;  /* Scanning line finished */
    }
    if (valid_label_name(trimmed_line,EXTERN,line,errors_found) != 0)
        return;  /* Scanning line finished */

    label = add_label(trimmed_line,0,EXTERN,TBD);
    if (label == NULL) {  /* Indicates memory allocation failed */
        fclose(line->file);
        free_line(line);
        free_labels();
        free_macros();
        free_all_memory();
        exit(1);  /* Exiting program */
    }
    line->label = label;  /* Setting the label pointer of struct line to the new entry label */
}

void valid_operation(unsigned short *code, int *Usage, int *IC, Line *line, char *ptr, int ind, int *errors_found) {
    char *opernad, *second_operand, *comma_pos;
    int operands_num = OPCODES[ind].operands_num, length, method, method_2;
    unsigned short word = 0;
    word |= (ind << SHIFT_OPCODE_POS) | BIT_ABSOLUTE_FLAG;

    /* Analyzing opernads */
    switch (operands_num) {
        case 0:
            if (ptr[0] != NULL_TERMINATOR) {  /* Checking if there is a extraneous text */
                print_syntax_error(Error_49,line->file_am_name,line->line_num);
                *errors_found = 1;
                return;  /* Scanning line finished */
            }
            add_instruction_code(code,Usage,IC,word,errors_found);  /* Adding machine code */
            return;  /* Scanning line finished */
        case 1:
            if (ptr[0] == NULL_TERMINATOR) {  /* Checking if there is a missing operand */
                print_syntax_error(Error_50,line->file_am_name,line->line_num);
                *errors_found = 1;
                return;  /* Scanning line finished */
            }
            while (*ptr && isspace(*ptr))  /* Skipping leading whitespace */
                ptr++;

            if (ptr[0] == COMMA) {  /* Checking if there is an illegal comma */
                print_syntax_error(Error_53,line->file_am_name,line->line_num);
                *errors_found = 1;
                return;  /* Scanning line finished */
            }
            if (contains_whitespace(ptr) || strchr(ptr,COMMA) != NULL) {  /* Checking for extraneous text */
                print_syntax_error(Error_52,line->file_am_name,line->line_num);
                *errors_found = 1;
                return;  /* Scanning line finished */
            }
            /* Checking if the addressing method is legal */
            method = which_addressing_method(ptr,line,errors_found);
            if (method == -1) {
                return;  /* Scanning line finished */
            }
            if (is_method_legal(line,method,ind,operands_num,errors_found) != 0) {
                return;  /* Scanning line finished */
            }
            handle_one_operand(code,Usage,IC,line,method,ptr,ind,errors_found);
            return;  /* Scanning line finished */
        case 2:
            if (ptr[0] == NULL_TERMINATOR) {  /* Checking if there are missing operands */
                print_syntax_error(Error_51,line->file_am_name,line->line_num);
                *errors_found = 1;
                return;  /* Scanning line finished */
            }
            while (*ptr && isspace(*ptr))  /* Skipping leading whitespace */
                ptr++;

            if (ptr[0] == COMMA) {  /* Checking if there is an illegal comma */
                print_syntax_error(Error_53,line->file_am_name,line->line_num);
                *errors_found = 1;
                return;  /* Scanning line finished */
            }
            opernad = get_first_word(ptr);
            if (opernad == NULL) {  /* Indicates memory allocation failed (all the other allocations were freed inside function) */
                fclose(line->file);
                free_line(line);
                exit(1);  /* Exiting program */
            }
            length = strlen(opernad);

            comma_pos = strchr(opernad,COMMA);
            if (comma_pos != NULL) {  /* If a comma was found, spliting the string into two opernads */
                if (comma_pos - opernad == strlen(opernad) -1 && ptr[length] == NULL_TERMINATOR) {
                    print_syntax_error(Error_50,line->file_am_name,line->line_num);
                    *errors_found = 1;
                    deallocate_memory(opernad);
                    return;  /* Scanning line finished */
                }
                *comma_pos = NULL_TERMINATOR;
                length = comma_pos - opernad;
                second_operand = &ptr[length+1];  /* Setting a pointer to the second operand */

                while (*second_operand && isspace(*second_operand))  /* Skipping leading whitespace */
                    second_operand++;

                if (second_operand[0] == COMMA) {  /* Checking if there are consecutive commas */
                    print_syntax_error(Error_55,line->file_am_name,line->line_num);
                    *errors_found = 1;
                    deallocate_memory(opernad);
                    return;  /* Scanning line finished */
                }
                if (contains_whitespace(second_operand) || strchr(second_operand,COMMA) != NULL) {  /* Checking for extraneous text */
                    print_syntax_error(Error_54,line->file_am_name,line->line_num);
                    *errors_found = 1;
                    deallocate_memory(opernad);
                    return;  /* Scanning line finished */
                }
            } else {  /* No comma was found in the first string */
                ptr = &ptr[length];
                while (*ptr && isspace(*ptr))  /* Skipping leading whitespace */
                    ptr++;
                if (ptr[0] == NULL_TERMINATOR || (*ptr == COMMA && ptr[1] == NULL_TERMINATOR)) {  /* Checking if there is a missing operand */
                    print_syntax_error(Error_50,line->file_am_name,line->line_num);
                    *errors_found = 1;
                    deallocate_memory(opernad);
                    return;  /* Scanning line finished */
                }
                if (ptr[0] != COMMA) {
                    print_syntax_error(Error_56,line->file_am_name,line->line_num);
                    *errors_found = 1;
                    deallocate_memory(opernad);
                    return;  /* Scanning line finished */
                }
                ptr++;
                while (*ptr && isspace(*ptr))  /* Skipping leading whitespace */
                    ptr++;
                second_operand = get_first_word(ptr);
                if (second_operand == NULL) {  /* Indicates memory allocation failed (all the other allocations were freed inside function) */
                    fclose(line->file);
                    free_line(line);
                    exit(1);  /* Exiting program */
                }
                if (second_operand[0] == COMMA) {
                    print_syntax_error(Error_55,line->file_am_name,line->line_num);
                    *errors_found = 1;
                    deallocate_memory(opernad);
                    deallocate_memory(second_operand);
                    return;  /* Scanning line finished */
                }
                length = strlen(second_operand);
                if (strchr(second_operand,COMMA) != NULL || ptr[length] != NULL_TERMINATOR) {  /* Checking for extraneous text */
                    print_syntax_error(Error_54,line->file_am_name,line->line_num);
                    *errors_found = 1;
                    deallocate_memory(opernad);
                    deallocate_memory(second_operand);
                    return;  /* Scanning line finished */
                }
            }
            method = which_addressing_method(opernad,line,errors_found);
            method_2 = which_addressing_method(second_operand,line,errors_found);
            if (method == -1 || method_2 == -1) {
                deallocate_memory(opernad);
                deallocate_memory(second_operand);
                return;  /* Scanning line finished */
            }
            if (is_method_legal(line,method,ind,operands_num,errors_found) != 0 ||  /* Checking if the addressing method is legal */
            is_method_legal(line,method_2,ind,operands_num-1,errors_found) != 0) {  /* operands_num-1 to signal that opernd is of type "destination" */
                deallocate_memory(opernad);
                deallocate_memory(second_operand);
                return;  /* Scanning line finished */
            }
            handle_two_operands(code,Usage,IC,line,opernad,second_operand,ind,errors_found);
            deallocate_memory(opernad);
            deallocate_memory(second_operand);
    }
}

void analyze_numbers(unsigned short *data, int *Usage, int *DC, Line *line, char *ptr, int *errors_found) {
    int *num_array;
    int num_count = 0, i =0;

    num_array = get_numbers(line,ptr,&num_count,errors_found);
    if (num_array == NULL) {
        if (line->label != NULL)
            remove_last_label();
        return;
    }
    /* Updating label properties */
    if (line->label != NULL) {
        line->label->address = *DC;
        line->label->location = DATA;
    }
   /* Adding machine code to data array */
    for (; i<num_count; i++) {
        if (*Usage == CAPACITY) {  /* Checking if memory limit was reached */
            print_system_error(Error_73);
            *errors_found = 1;
            (*Usage)++;  /* Incrementing usage count so the next iteration will not print another error message */
            return;  /* Scanning line finished */
        }
        if (*Usage > CAPACITY) {  /* Checking if memory limit was exceeded */
            return;  /* Scanning line finished */
        }
        add_data_code(data,DC,num_array[i]);  /* Adding machine code */
        (*Usage)++;  /* Incrementing usage count */
    }
    deallocate_memory(num_array);
}