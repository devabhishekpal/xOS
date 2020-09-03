#include "kernel.h"

extern void call_c_test(void);

void kernel_main(unsigned long multiboot_magic, multiboot_info_t *multiboot_info){
    initialize_memory();

    term_setup();
    initialize_gui();
    
    printf("::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    printf("::::::     '##::::'##::'#######:::'######::     ::::::\n");
    printf("::::::     . ##::'##::'##.... ##:'##... ##:     ::::::\n");
    printf("::::::     :. ##'##::: ##:::: ##: ##:::..::     ::::::\n");
    printf("::::::     ::. ###:::: ##:::: ##:. ######::     ::::::\n");
    printf("::::::     :: ## ##::: ##:::: ##::..... ##:     ::::::\n");
    printf("::::::     : ##:. ##:: ##:::: ##:'##::: ##:     ::::::\n");
    printf("::::::      ##:::. ##:. #######::. ######::     ::::::\n");
    printf("::::::     ..:::::..:::.......::::......:::     ::::::\n");
    printf("::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    printf("A custom \"OS\" made with Love by ABHISHEK PAL\n\n");

    initialize_keyboard();
    initialize_interrupts();
    initialize_processes();
    initialize_commands();

    printf("Initialized!!!\n");
    printf("    multiboot_magic:    0x%08X\n", multiboot_magic);
    printf("    multiboot_address:  0x%08X\n", multiboot_info);
    printf("    vbe_mode:           0x%08X\n", multiboot_info->vbe_mode);
    printf("    mem_upper:          ");
    printfcomma( multiboot_info->mem_upper );
    printf("k\n");
    printf("    mem_top:            0x%08X\n", get_mem_top());

    if(!is_gui_active()){
        printf("\n");
    }

    launch_commands_loop();

    printf("Error: End of kernel_main() reached!!!\n");

}

void call_asm_test(void){
    printf("\nCalled from ASM...\n");
}

void echo_registers(void){
    printf("\neax:  0x%X\nebx:  0x%X\necx:  0x%X\nedx:  0x%X\n");
}
