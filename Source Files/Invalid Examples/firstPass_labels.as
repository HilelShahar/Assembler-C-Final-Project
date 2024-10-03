;Invalid label declaration, label name is too long, 31 characters max
thisLabelNameIsWayTooLongAndInvalidForUse: mov r1, r2

;Invalid label declaration, label name must contain alphabetic
;or numeric characters only
label_with_invalid_chars_@: add r2, r3

;Invalid label declaration, no value associated with label
labelWithNoValue:

;Invalid label declaration, a label name cannot be the same as a macro name
macr myMacro
    sub r1, r2
endmacr
myMacro: clr r3

;Invalid label declaration, an opcode name cannot be used as a label name
mov: inc r4

;Invalid label declaration, a register name cannot be used as a label name
r0: dec r5

;Invalid label declaration, an instruction name cannot be used as a label name
.data: jmp r6

;Invalid label declaration, this label name is already in use
duplicateLabel: inc r1
duplicateLabel: add r2, r3

;Invalid label declaration, no name was defined
: sub r4, r5

;Invalid label declaration, label name must start with an alphabetical character
1invalidLabelName: prn r6

;Invalid label declaration, "macr" is a reserved word
macr: stop

;Invalid label declaration, "endmacr" is a reserved word
endmacr: rts

;Invalid label declaration, local label name cannot be the same
;as an external label name
.extern externalLabel
externalLabel: mov r1, r2

;This macro call was missed in pre-processing, macros calls are
;allowed once per line and with no extra text
myMacro mov r1, r2
