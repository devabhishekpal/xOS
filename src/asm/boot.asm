;CONSTS for MULTIBOOT HEADER
MULTIBOOTALIGN equ 1<<0                                                 ;Align loaded modules to page bounds
MULTIBOOTMEMINFO equ 1<<1                                               ;Giving a memory map
MULTIBOOT_VBE_MODE equ 1<<2                                             ;Giving the graphics mode
FLAGS equ MULTIBOOTALIGN | MULTIBOOTMEMINFO | MULTIBOOT_VBE_MODE        ;Multiboot FLAGS
MAGIC equ 0x1BADB002                                                    ;'magic' value to allow finding of header
CHECKSUM equ -( MAGIC + FLAGS )                                         ;CHECKSUM to verify we are multiboot

section .multiboot
    align 4                                         ;MULTIBOOT specs require we align to 4bytes i.e offset by 4bytes to page bound
        dd MAGIC
        dd FLAGS
        dd CHECKSUM

        dd 0                                        ;header_addr
        dd 0                                        ;load_addr
        dd 0                                        ;load_end_addr
        dd 0                                        ;bss_end_addr
        dd 0                                        ;entry_addr

        dd 0                                        ;mode_type
        dd 1280                                     ;width
        dd 720                                      ;height
        dd 32                                       ;depth

section .bss
    align 4
    stack_bottom:
        resb 16384                                  ;Reserving 16KB for stack size
    stack_top:

section .data
    align 4
    GDTR:                                           ;Global descriptor table register
        dw GDT_END-GDT-1                            ;END = GDT - 1
        dd GDT
    
    GDT:                                            ;Global Descriptor Table
        NULL_DESC equ $-GDT                         ;NULL descriptor required with 64bits per entry
            dd 0x0
            dd 0x0
        CODE_SEL equ $-GDT                          ;4GB flat code at 0x0 having 0xfffff limit
            dw 0xFFFF                               ;Limit(2):0xffff
            dw 0x0                                  ;Base(3)
            db 0x0                                  ;Base(2)
            db 0x9A                                 ;Type: present, ring0, code, exec/read/write/accessed (10011010)
            db 0xCF                                 ;Limit(1): 0xf | Flags: 4KB incl 32bit (11001111)
            db 0x0                                  ;Base(1)
        DATA_SEL equ $-GDT                          ;4GB flat data at 0x0 having 0xfffff limit
            dw 0xFFFF                               ;Limit(2):0xffff
            dw 0x0                                  ;Base(3)
            db 0x0                                  ;Base(2)
            db 0x92                                 ;Type: present, ring0, data/stack, read/write (10010010)
            db 0xCF                                 ;Limit(1):0xf | Flags: 4KB incl 32bit (11001111)
            db 0x0                                  ;Base(1)
    GDT_END:

section .text
    global _step_one:function (_step_two.end - _step_one)
    _step_one:
        mov esp, stack_top                          ;move esp segment register to stack top for OS stack
        cli                                         ;clear interrupts
        lgdt [GDTR]                                 ;load global descriptor table given by the GDTRegister

        mov ax, 0x10                                ;move 0x10 (10000) to ax i.e offset for GDT
        ;;Fill the 32bit r/w data segments with 0x10
        mov ds, ax                                  
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax

        jmp 0x08:_step_two                          ;0x08 is the offset for the jump

    _step_two:
        push ebx
        push eax
        extern kernel_main
        call kernel_main

        cli
    .hang:
        hlt
        jmp .hang

    .end:
