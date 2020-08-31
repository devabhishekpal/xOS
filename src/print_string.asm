;;;Prints characters in bx register

print_string:
    pusha                       ;Push all registers into stack

;;Added new labels as pushing registers onto stack and then again moving values
;;inside ax will cause characters and values to overlap

print_characters:
    mov al, [bx]                ;copy character value at bx to al
    cmp al, 0
    je end_print                ;jmp if (al=0)
    int 0x10                    ;print character in al (interrupt)
    add bx, 1                   ;Add 1byte to bx i.e get the next character
    jmp print_characters        ;loop over

end_print:
    popa                        ;restore registers from the stack
    ret                         ;return to caller
