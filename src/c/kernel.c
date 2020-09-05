#include "kernel.h"

extern void call_c_test(void);

void kernel_main(unsigned long multiboot_magic, multiboot_info_t *multiboot_info){
    init_multiboot(multiboot_magic, multiboot_info);
    init_memory();
    init_vga();
    term_setup();
    init_gui();
    
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

    init_keyboard();
    init_timer();
    init_interrupts();
    init_processes();
    init_commands();

/*
    printf("Initialized: Memory!!!\n");
    printf("    multiboot_magic:    0x%08X\n", multiboot_magic);
    printf("    multiboot_address:  0x%08X\n", multiboot_info);
    printf("    fbuff_addr:         0x%08X\n", multiboot_info->framebuffer_addr);
    printf("    mem_upper:          ");
    printfcomma( multiboot_info->mem_upper );
    printf("k\n");
    printf("    mem_top:            0x%08X\n", get_mem_top());
*/

    vga_info *vga = get_vga_info();

    printf( "Width:           %d\n", vga->width );
	printf( "Height:          %d\n", vga->height );
	printf( "Pixel Width:     %d\n", vga->pixel_width );
	printf( "Pitch:           %d\n", vga->pitch );
	printf( "fbuff:           0X%08X\n", vga->fbuffer );

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
