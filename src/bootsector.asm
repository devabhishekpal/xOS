;;; Basic boot sector

    org 0x7c00                  ;'origin' of bootcode. Makes sure address dont change
    ;;All the following are offset from 0x7c00
;;Setting up modes
    mov ah, 0x00                ;int 0x10/ah 0x00 = set video mode
    mov al, 0x03                ;sets to 88x25 text mode
    int 0x10                    ;calling interrupt for the execution
;;Change colour
    mov ah, 0x0b                ;int 0x10/ah 0x0b = set background color
    mov bh, 0x00                ;sets background color/ border color
    mov bl, 0x01                ;actual colour to be set
    int 0x10                    ;call to execute

;;Output start
    mov ah, 0x0e                ;int 10/ ah 0x0e teletype output
    mov bx, testString          ;Copying mem address of testString into bx
    
    call print_string           ;call print_string as a function rather than a loop using jump
    mov bx, nextString
    call print_string

    ;;End program
    jmp $                       ;loop infinitely to here $ sign mean current memory offset
    include "print_string.asm"

testString: db 'TEST',0xa, 0xd, 0        ;0/null means terminate string with null
;;0xa is the next line and 0xd is for carriage feed which means that it will move cursor to beginning of the next line
nextString: db 'NEXT STRING', 0


    ;;Magic number
    times 510-($-$$) db 0       ;inserts 0s upto 510 bytes
    dw 0xaa55                   ;BOOT magic number
    