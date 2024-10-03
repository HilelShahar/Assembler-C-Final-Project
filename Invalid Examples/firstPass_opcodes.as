;This operation has extraneous text, no operands required
stop extra_text_here

;This operation has a missing operand
mov r1,

;This operation has missing operands
cmp

;This operation has extraneous text, only one operand is required
clr r1, extra_text_here

;This operation has an illegal comma after the operation name
mov ,r1,r2

;This operation has extraneous text, only two operands are required
add r1, r2, extra_text_here

;This operation has multiple consecutive commas
sub r1,,r2

;This operation has a missing comma
lea r1 r2

;This operation uses an illegal method for a destination operand
mov #5, #10

;This operation uses an illegal method for a source operand
lea *r1, r2

;This operand was recognized as a label but is not valid, please check syntax
mov invalid_label, r1

;Operand that uses an 'IMMEDIATE' method type but has no value
add #, r1

;This operand is invalid for an 'IMMEDIATE' method type, only integers allowed
add #Nope, r2

;This operand is out of range for an 'IMMEDIATE' method type
sub #32768, r3

;Operand that uses an 'INDIRECT_REGISTER' method type but has no value
mov * , r1

;This operand is not a valid register for an 'INDIRECT_REGISTER' method type
add *r8, r1

;Unrecognized command, label declarations must have a space after the colon (:)
LABEL:mov r1, r2

;Unrecognized command, label declarations must have the (:) attached to the name
LABEL : mov r1, r2

;Unrecognized command, instruction must start with a dot
entry LABEL

;Unrecognized command, please check syntax
unrecognizedCommand r1, r2

;Invalid operand, reserved words and macro names cannot be used as operands
macr myMacro
    add r1, r2
endmacr
mov myMacro, r3

;Label name is not a valid command
LABEL: inc *r1
LABEL r7
