#include "kernel.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

unsigned int inp_x;

bool _is_gui_active = false;

unsigned short *term_buffer; //Holds the display characters

void init_gui(void){
    term_buffer = (unsigned short *)0xB8000;
    inp_x = 0;
    _is_gui_active = true;
    term_setup_gui();
    draw_base();
    draw_menu();
    gui_update_carret(24, inp_x);
}

bool is_gui_active(void){
    return _is_gui_active;
}

void draw_base(void){
    unsigned int x;
    unsigned int y;

    //Menu area
    for(x = 0; x < 80; x++){
        term_buffer[(0*SCREEN_WIDTH) + x] = vga_entry(' ', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    }

    //Main content area
    for(y = 1; y < 22; y++){    //Height
        for(x = 0; x < 80; x++){    //Width
            term_buffer[(y*SCREEN_WIDTH) + x] = vga_entry(' ', vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE));
        }
    }

    //Draw empty line
    for(x = 0; x < 80; x++){
        term_buffer[(23*SCREEN_WIDTH) + x] = vga_entry(196, vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE));
    }

    //Draw area for command entry
    for(x = 0; x < 80; x++){
        term_buffer[(24*SCREEN_WIDTH) + x] = vga_entry(' ', vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE));
    }
}

void draw_menu(void){
    gui_write_string_at("F1 -> Go Multi-Task   |   F2 -> PCI   |   F3 -> Memory Exp", vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK), 1, 0);
}

void gui_reset_input(void){
    unsigned int x;
    for(x = 0; x < 80; x++){
        term_buffer[(24*SCREEN_WIDTH) + x] = vga_entry(' ', vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE));  //Fill the area with color scheme fg = black and bg = white i.e same as the clear area
    }

    inp_x = 0;
    gui_update_carret(24, inp_x);   //Reset position of cursor to 0
}

void gui_put_char_at_input(char user_inp){
    term_put_char_at(user_inp, vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE), inp_x++, 24);
    gui_update_carret(24, inp_x);   //Shift cursor to next position
}

void gui_update_carret(int row, int col){

    //Same implementation as update_cursor in base.c
    unsigned short pos = (row*80) + col;    //y = (row * width) + col

    //Cursor LOW port to VGA index register
    outportb(0x3D4, 0x0F);
    outportb(0x3D5, (unsigned char)(pos&0xFF));

    //Cursow HIGH port to VGA index register
    outportb(0x3D4, 0x0E);
	outportb(0x3D5, (unsigned char)((pos>>8)&0xFF));
}

void gui_put_char_at(char c, unsigned char color, unsigned int x, unsigned int y){
    term_put_char_at(c, color, x, y);
}

void gui_put_sting_at(const char *data, size_t size, unsigned char color, unsigned int x, unsigned int y){  //Draw string at position by putting each character at the placce
    for(size_t i = 0; i < size; i++){
        gui_put_char_at(data[i], color, x+i, y);    //x+i = postion in x axes goes by +1 on each character and y is y axes
    }
}

//Gives the data string and its size
void gui_write_string_at(const char *data, unsigned char color, unsigned int x, unsigned int y){
    gui_put_sting_at(data, strlen(data), color, x, y);
}