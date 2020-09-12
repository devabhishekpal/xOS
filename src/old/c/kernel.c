#include "kernel.h"

extern void call_c_test(void);

/*
* Beginning of the kernel
* params
*   multiboot_magic -> The multiboot magic number
*   multiboot_info  -> Information about the multiboot
*/
void kernel_main(unsigned long multiboot_magic, multiboot_info_t *multiboot_info){
    init_multiboot(multiboot_magic, multiboot_info);
    init_memory();
    init_vga();
    term_setup();
    init_gui();
    
    window *win = window_new("xOS console", 399, 105, (7 * 80 + 3), (14 * 25 + 1 +25));
    draw_window(win);

    console *con = console_new(400, 130, (7 * 80), (14 * 25), color_cntrl_background, color_cntrl_text);
    console_set_def(con);
    draw_console(con);
    
    printf("\n\n");
    printf("          .d88888b.   .d8888b.  \n");
    printf("         d88P. .Y88b d88P  Y88b \n");
    printf("         888     888 Y88b.      \n");
    printf("888  888 888     888  .Y888b.   \n");
    printf(".Y8bd8P. 888     888     .Y88b. \n");
    printf("  X88K   888     888       .888 \n");
    printf(".d8..8b. Y88b. .d88P Y88b  d88P \n");
    printf("888  888  .Y88888P.   .Y8888P.  \n\n");
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

    printf("Width:           %d\n", vga->width);
	printf("Height:          %d\n", vga->height);
	printf("Pixel Width:     %d\n", vga->pixel_width);
	printf("Pitch:           %d\n", vga->pitch);
	printf("fbuff:           0X%08X\n", vga->fbuffer);
    printf("\n");

    launch_commands_loop();

    printf("Error: End of kernel_main() reached!!!\n");

}

void call_asm_test(void){
    printf("\nCalled from ASM...\n");
}
