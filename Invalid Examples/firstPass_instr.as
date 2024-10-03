;Instruction ".data" has an illegal comma following the instruction name
.data , 5, 10

;Instruction ".data" has illegal characters, only integers allowed
.data 5, 10, A, 20

;Instruction ".data" has a missing comma
.data 5 10, 15

;Instruction ".data" has multiple consecutive commas
.data 5,,10,15

;Instruction ".data" expects an integer after the last comma
.data 5, 10,

;Instruction ".data" has a number that is out of range
.data 32768

;Instruction ".data" has no parameters
.data

;Instruction ".string" expects one string parameter enclosed in double quotes
.string HelloWorld

;Instruction ".string" has no parameter
.string

;Instructions ".entry" and ".extern" must have a label declaration
.entry
.extern

;Instructions ".entry" and ".extern" allow one label declaration at a time
.entry LABEL1, LABEL2
.extern LABEL1, LABEL2

;Instructions ".entry" and ".extern" label name must refer to a valid label
.entry invalid_Label
.extern invalid_Label

;Instructions ".entry" and ".extern" label name cannot be the same as a macro
macr myMacro
    add r1, r2
endmacr
.entry myMacro
.extern myMacro

;Instruction ".entry" label name cannot be the same as an external label name
.extern externalLabel
.entry externalLabel

;Instruction ".extern" label name cannot be the same as a local label name
LABEL1: mov r1, r2
.extern LABEL1

.entry LABEL2
.extern LABEL2
