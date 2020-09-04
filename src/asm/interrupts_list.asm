;;;contains all system exception interrupts
;;;and other system interface interrupts
;;;note: exception 15 and 20-31 are Intel Reserved


align 4

global call_c_test

extern call_asm_test
extern interrupt_default_handler
extern keyboard_test

section .text

global sys_break
sys_break:
    mov eax, 0x1000
    int 0x30
    ret

call_c_test:
    mov eax, 0xBADA
    int 0x30
    ret

[global int30]
int30:
    pushad
        push ds
        push es
        push fs
        push gs
            mov eax, 0x10                                       ;Data segment
            mov ds, eax
            mov es, eax
            cld                                                 ;Clear direction flag
            call interrupt_default_handler
        pop gs
        pop fs
        pop es
        pop ds
    popad
    iret                                                        ;return interrupt

[extern int_00]
[global int00]
int00:
    pusha
        push ds
        push es
        push fs
        push gs
            mov eax, 0x10                                           ;Data segment
            mov ds, eax
            mov es, eax
            cld
            call int_00                                             ;Divide by 0
        pop gs
        pop fs
        pop es
        pop ds
    popa
    iret

[extern int_01]
[global int01]

int01:
    pusha
        push ds
        push es
        push fs
        push gs
            mov eax, 0x10                                           ;Data segment
            mov ds, eax
            mov es, eax
            cld
            call int_01                                             ;Debug
        pop gs
        pop fs
        pop es
        pop ds
    popa
    iret

[extern int_02]
[global int02]

int02:
    pusha
        push ds
        push es
        push fs
        push gs
            mov eax, 0x10                                           ;Data segment
            mov ds, eax
            mov es, eax
            cld
            call int_02                                             ;Non maskable interrupt
        pop gs
        pop fs
        pop es
        pop ds
    popa
    iret

[extern int_03]
[global int03]
int03:
     pusha
        push ds
        push es
        push fs
        push gs
            mov eax,0x10    ; Data segment
            mov ds,eax
            mov es,eax
            cld
            call int_03    ; Breakpoint
        pop gs
        pop fs
        pop es
        pop ds
     popa
     iret

[extern int_04]
[global int04]
int04:
     pusha
        push ds
        push es
        push fs
        push gs
            mov eax,0x10    ; Data segment
            mov ds,eax
            mov es,eax
            cld
            call int_04    ; Overflow
        pop gs
        pop fs
        pop es
        pop ds
     popa
     iret

[extern int_05]
[global int05]
int05:
     pusha
        push ds
        push es
        push fs
        push gs
            mov eax,0x10    ; Data segment
            mov ds,eax
            mov es,eax
            cld
            call int_05    ; Bound Range Exception
        pop gs
        pop fs
        pop es
        pop ds
     popa
     iret

[extern int_06]
[global int06]
int06:
     pusha
        push ds
        push es
        push fs
        push gs
            mov eax,0x10    ; Data segment
            mov ds,eax
            mov es,eax
            cld
            call int_06    ; Invalid Opcode
        pop gs
        pop fs
        pop es
        pop ds
     popa
     iret

[extern int_07]
[global int07]
int07:
     pusha
        push ds
        push es
        push fs
        push gs
            mov eax,0x10    ; Data segment
            mov ds,eax
            mov es,eax
            cld
            call int_07    ; Device Not Available
        pop gs
        pop fs
        pop es
        pop ds
     popa
     iret

[extern int_08]
[global int08]
int08:
     pusha
        push ds
        push es
        push fs
        push gs
            mov eax,0x10    ; Data segment
            mov ds,eax
            mov es,eax
            cld
            call int_08    ; Double Fault
        pop gs
        pop fs
        pop es
        pop ds
     popa
     iret


[extern int_09]
[global int09]
int09:
     pusha
        push ds
        push es
        push fs
        push gs
            mov eax,0x10    ; Data segment
            mov ds,eax
            mov es,eax
            cld
            call int_09    ; Coprocessor Segment Overrun
        pop gs
        pop fs
        pop es
        pop ds
     popa
     iret

[extern int_10]
[global int10]
int10:
     pusha
        push ds
        push es
        push fs
        push gs
            mov eax,0x10    ; Data segment
            mov ds,eax
            mov es,eax
            cld
            call int_10    ; Invalid Task State Segment (TSS)
        pop gs
        pop fs
        pop es
        pop ds
     popa
     iret

[extern int_11]
[global int11]
int11:
     pusha
        push ds
        push es
        push fs
        push gs
            mov eax,0x10    ; Data segment
            mov ds,eax
            mov es,eax
            cld
            call int_11    ;  Segment Not Present
        pop gs
        pop fs
        pop es
        pop ds
     popa
     iret

[extern int_12]
[global int12]
int12:
     pusha
        push ds
        push es
        push fs
        push gs
            mov eax,0x10    ; Data segment
            mov ds,eax
            mov es,eax
            cld
            call int_12    ; Stack Segment Fault
        pop gs
        pop fs
        pop es
        pop ds
     popa
     iret

[extern int_13]
[global int13]
int13:
     pusha
        push ds
        push es
        push fs
        push gs
            mov eax,0x10    ; Data segment
            mov ds,eax
            mov es,eax
            cld
            call int_13    ; General Protection Fault (GPF)
        pop gs
        pop fs
        pop es
        pop ds
     popa
     iret

[extern int_14]
[global int14]
int14:
     pusha
        push ds
        push es
        push fs
        push gs
            mov eax,0x10    ; Data segment
            mov ds,eax
            mov es,eax
            cld
            call int_14    ; Page Fault
        pop gs
        pop fs
        pop es
        pop ds
     popa
     iret

;;;Exception 15 is an Intel Reserved Interrupt

[extern int_16]
[global int16]
int16:
     pusha
        push ds
        push es
        push fs
        push gs
            mov eax,0x10    ; Data segment
            mov ds,eax
            mov es,eax
            cld
            call int_16    ; x87 Floating-Point Error
        pop gs
        pop fs
        pop es
        pop ds
     popa
     iret

[extern int_17]
[global int17]
int17:
     pusha
        push ds
        push es
        push fs
        push gs
            mov eax,0x10    ; Data segment
            mov ds,eax
            mov es,eax
            cld
            call int_17    ; Alignment Check
        pop gs
        pop fs
        pop es
        pop ds
     popa
     iret

[extern int_18]
[global int18]
int18:
     pusha
        push ds
        push es
        push fs
        push gs
            mov eax,0x10    ; Data segment
            mov ds,eax
            mov es,eax
            cld
            call int_18    ; Machine Check
        pop gs
        pop fs
        pop es
        pop ds
     popa
     iret

[extern int_19]
[global int19]
int19:
     pusha
        push ds
        push es
        push fs
        push gs
        mov eax,0x10    ; Data segment
            mov ds,eax
            mov es,eax
            cld
            call int_19    ; SIMD Floating-Point Exception #XF
        pop gs
        pop fs
        pop es
        pop ds
     popa
     iret

;;;Exceptions 20-31 are Intel Reserved Interrupts

;;;End of Exceptions/Interrupts



[global int21]
int21:
     pusha
          push ds
          push es
          push fs
          push gs
               mov eax,0x10    ; Data segment
               mov ds,eax
               mov es,eax
               cld
               call keyboard_test   ; Test Interrupt Handler
          pop gs
          pop fs
          pop es
          pop ds
     popa
     iret