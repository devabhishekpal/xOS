#include "kernel.h"

unsigned int term_curr_row;
unsigned int term_curr_col;
unsigned int term_curr_color;
unsigned short *term_buffer;

unsigned int VGA_WIDTH = 80;
unsigned int VGA_HEIGHT = 25;
unsigned int y_start;

//Start terminal
//-> Define default colors
//->Clear screen

void term_setup(void){
    unsigned int x = 0;
    unsigned int y = 0;

    term_curr_row = 0;
    term_curr_col = 0;
    y_start = 0;
    term_curr_color = vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
    term_buffer = (unsigned short*)0xB8000;

    for(y = 0; y < VGA_HEIGHT; y++){
        for(x = 0; x < VGA_WIDTH; x++){
            term_buffer[(y * VGA_WIDTH) + x] = vga_entry(' ', term_curr_color);
        }
    }
    init_printf(NULL, putc); //Initialize printf
}

void term_setup_gui(void){
    y_start = 1;
    VGA_HEIGHT = 24;    //Leave a row
    term_curr_row = 1;
}

//Set color for the terminal
void term_set_color(unsigned char color){
    term_curr_color = color;
}

//Put character at x,y with color without advancing buffer
void term_put_char_at(char c, unsigned char color, unsigned int x, unsigned int y){
    color = 0;
    if(c != '\n'){
        
        if(is_gui_active()){
            gui_put_char_at(c, x, y);
        }
        else{
            term_buffer[(y*VGA_WIDTH) + x] = vga_entry(c, color);
        }
    }
}

//Put character at current position and advance the postion
void term_put_char(char c){
    unsigned int x;
    unsigned int y;
    unsigned int max_row;

    if(c != '\n'){
        term_put_char_at(c, term_curr_color, term_curr_col, term_curr_row);
    }
    else{
        term_curr_col = VGA_WIDTH - 1;  //One col filled and hence reduce width by 1
    }
    term_curr_col++;
    if(term_curr_col == VGA_WIDTH){
        term_curr_col = 0;  //Reset the cursor to start
        term_curr_row++;    //Change row to next line
        if(is_gui_active()){
            max_row = VGA_HEIGHT;
        }
        else{
            max_row = VGA_HEIGHT;
        }
        if(term_curr_row == max_row){
            term_curr_row = max_row - 1;
            for(y = y_start; y < max_row - 1; y++){
                if(is_gui_active()){
                    gui_move_row(y, y + 1);
                }
                else{
                    for(x = 0; x < VGA_WIDTH; x++){
                        term_buffer[(y * VGA_WIDTH) + x] = term_buffer[((y+1) * VGA_WIDTH) + x];
                    }
                }
            }
			for( x = 0; x < VGA_WIDTH; x++ ) {
				if(is_gui_active()){
                    gui_put_char_at(' ', x, VGA_HEIGHT - 1);
                }
                else{
                    const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
				    term_buffer[index] = vga_entry(' ', term_curr_color );
                }
			}
        }
    }
    update_cursor(term_curr_row, term_curr_col);
}

void term_clear_last_char(void){
    int term_new_col = term_curr_col - 1; //Move postion one space behind
    if(is_gui_active()){
        gui_clear_last_input_char();
    }
    else{
        if(term_new_col > 0){
            term_put_char_at(' ', term_curr_color, term_new_col, term_curr_row);    //Fill the col with blank char
            term_curr_col = term_new_col;
        }
    }
    update_cursor(term_curr_row, term_curr_col);
}

//Gives a string of the terminal size
void term_put_string(const char *data, size_t size){
    for(size_t i = 0; i < size; i++){
        term_put_char(data[i]); //Put the character one by one
    }
}

//Calculate size of *data and give the string *data
void term_write_string(const char *data){
    term_put_string(data, strlen(data));
}
