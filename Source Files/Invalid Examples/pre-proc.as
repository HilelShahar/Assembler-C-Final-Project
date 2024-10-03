      Thisssssss lliiiinnneeeee iss wayyyyyyyy tooooo longgggggggggg      TTTTThissssss lliiiinnneeeeeeee isssss wayyyyyyyy tooooo lllllongggggggggggg!!!!!

;Invalid macro declaration, no name was defined
macr
endmacr

;Invalid macro declaration, "macr" command with extraneous text
e macr macro_name
endmacr

;Invalid macro declaration, an opcode name cannot be used as a macro name
macr mov
endmacr

;Invalid macro declaration, a register name cannot be used as a macro name
macr r0
endmacr

;Invalid macro declaration, an instruction name cannot be used as a macro name
macr .data
endmacr

;Invalid macro declaration, macro should be assigned with one name
macr macro1 macro2
endmacr

;Invalid macro declaration, this macro name is already in use
macr mymacro
    mov r1, r2
endmacr

macr mymacro
    add r1, r3
endmacr

;Invalid macro declaration, "endmacr" command with extraneous text
macr anothermacro
    sub r2, r4
endmacr extra_text_here

;Invalid macro declaration, macro name is too long, 31 characters max
macr thisMacroNameIsWayTooLongAndInvalid
    clr r5
endmacr

;Invalid macro declaration, empty macro content
macr emptyMacro

endmacr

;Invalid macro declaration, "macr" is a reserved word
macr macr
    inc r1
endmacr

;Invalid macro declaration, "endmacr" is a reserved word
macr endmacr
    dec r2
endmacr
