/**
 * This is the definitions header file.
 */
#ifndef DEFINITIONS_H
#define DEFINITIONS_H


/* Constants */

#define EXTENSION_LEN 3
#define MAX_LINE_LENGTH 81
#define MAX_MACRO_LENGTH 31
#define MAX_LABEL_LENGTH 31
#define OPCODES_COUNT 16
#define REGISTERS_COUNT 8
#define INSTRUCTIONS_COUNT 4
#define MACR_LENGTH 4
#define ENDMACR_LENGTH 7
#define CAPACITY 4096
#define STARTING_ADDRESS 100
#define DECIMAL_BASE 10
#define BIT_MASK 1
#define TWO 2
#define SHIFT_OPCODE_POS 11
#define SHIFT_SRC_OPERAND 7
#define SHIFT_DST_OPERAND 3
#define SHIFT_SRC_REGISTER 6
#define SHIFT_DST_REGISTER 3
#define BIT_MASK_EXTERNAL 1
#define BIT_MASK_RELOCATABLE 2
#define BIT_MASK_SIGNAL 3  /* Bits 0 for "External" and 1 for "Relocatable" to signal an uncoded label */
#define BIT_ABSOLUTE_FLAG 4
#define MASK_15BIT 0x7FFF
#define MASK_12BIT 0xFFF
#define MAX_15BIT 16383  /* Max number for a 15 bit 2's complement representation */
#define MIN_15BIT (-16384)  /* Min number for a 15 bit 2's complement representation */
#define MAX_12BIT 2047  /* Max number for a 12 bit 2's complement representation */
#define MIN_12BIT (-2048)  /* Min number for a 12 bit 2's complement representation */
#define BUFFER_SIZE 20  /* Sufficient size to temporarily store the string representation of an integer */
#define MAX_DATA_NUM 38  /* The max amount of numbers possible for a line of ".data" instruction, considering commas and max characters per line */
#define DOT '.'
#define HASH '#'
#define PLUS '+'
#define MINUS '-'
#define COLON ':'
#define COMMA ','
#define COMMENT ';'
#define ASTERISK '*'
#define DOUBLE_QUOTE '\"'
#define NULL_TERMINATOR '\0'


#endif