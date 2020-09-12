#include "kernel.h"

console *def_console;

unsigned int cursor_count;
bool _is_gui_active = false;

void init_gui(void){
    _is_gui_active = true;
    //draw_mouse(1280 / 2, 720 / 2);
    term_setup_gui();
}

bool is_gui_active(void){
    return _is_gui_active;
}

void gui_move_row(unsigned int dest, unsigned int src){
    vga_move_line(dest, src);
}

void gui_update_carret(void){
    console_put_char_at(def_console, '|', def_console->curr_x, def_console->curr_y);
    cursor_count = 1;
}

void gui_flip_cursor(void){
    if(is_vga_active()){
        if(cursor_count == 1){
            console_put_char_at(def_console, ' ', def_console->curr_x, def_console->curr_y);
            cursor_count = 0;
        }
        else{
            console_put_char_at(def_console, '|', def_console->curr_x, def_console->curr_y);
            cursor_count = 1;
        }
    }
}

void gui_put_char_at(char c, unsigned int x, unsigned int y){
    console_put_char_at(def_console, c, x, y);
}

void gui_put_char(char c){
    console_put_char(def_console, c);
}

void gui_put_sting_at(const char *data, size_t size, unsigned int x, unsigned int y){  //Draw string at position by putting each character at the placce
    for(size_t i = 0; i < size; i++){
        gui_put_char_at(data[i], x + i, y);    //x+i = postion in x axes goes by +1 on each character and y is y axes
    }
}

void gui_clear_last_char(void){
    console_clear_last_char(def_console);
}

//Gives the size(*data) and output(*data)
void gui_write_string_at(const char *data, unsigned int x, unsigned int y){
    gui_put_sting_at(data, strlen(data), x, y);
}

void gui_set_default_console_curr_row_col(unsigned int row, unsigned int col){
    def_console->prev_y = def_console->curr_y;
    def_console->prev_x = def_console->curr_x;
    def_console->curr_y = row;
    def_console->curr_x = col;

    gui_update_carret();
}

window *window_new(char *title, unsigned int x, unsigned int y, unsigned int width, unsigned int height){
    window *win = malloc(sizeof(window));

    win->box.x = x;
    win->box.y = y;
    win->box.w = width;
    win->box.h = height;

    win->bg_color = color_window;
    win->fg_color = color_window_box;

    strcpy(win->title, title);

    return win;
}

void draw_window(window *win){
    unsigned int x;
    unsigned int y;
    vga_info *vga_inf = get_vga_info();

    rect title_bar;

    title_bar.x = win->box.x;
    title_bar.y = win->box.y;
    title_bar.w = win->box.w;
    title_bar.h = 25;

    draw_rect(win->box, win->fg_color);
    draw_rect(title_bar, color_titlebar);
    draw_string(win->title, title_bar.x + 5, title_bar.y + 5, color_titlebar_text, color_titlebar); //Set padding of 5
}

void console_set_def(console *con){
    def_console = con;
}

console *console_new(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int bg_color, unsigned int fg_color){
    unsigned int _x, _y;
    console *con = malloc(sizeof(console));
    con->box.x = x;
    con->box.y = y;
    con->box.w = width;
    con->box.h = height;

    con->width = con->box.w / 7;
    con->height = con->box.h / 14;

    con->bg_color = bg_color;
    con->fg_color = fg_color;

    con->curr_x = 0;
    con->curr_y = 0;
    con->prev_x = 0;
    con->prev_y = 0;

    con->waiting_on_inp = false;

    con->buffer = malloc(sizeof(char) * con->width * con->height);  //Set buffer to size of console

    for(_y = 0; _y < con->height; _y++){
        for(_x = 0; _x < con->width; _x++){
            con->buffer[(_y * con->width) + _x] = ' ';
        }
    }
    return con;
}

void draw_console(console *con){
    unsigned int x;
    unsigned int y;
    vga_info *vga = get_vga_info();

    draw_rect(con->box, con->bg_color);

    for(y = 0; y < con->height; y++){
        for(x = 0; x < con->width; x++){
            draw_char(vga->fbuffer, con->box.x + (x * vga->char_width), con->box.y + (y * vga->char_height), con->fg_color, con->bg_color, con->buffer[(y * con->width) + x]);
        }
    }
}

void console_put_char_at(console *con, char c, unsigned int x, unsigned int y){
    vga_info *vga = get_vga_info();

    con->buffer[x + (con->width * y)] = c;
    draw_char(vga->fbuffer, con->box.x + (x * vga->char_width), con->box.y + (y * vga->char_height), con->fg_color, con->bg_color, c);
}

void console_put_char(console *con, char c){
    unsigned int x;
    unsigned int y;

    if(c != '\n'){
        console_put_char_at(con, c, con->curr_x, con->curr_y);
    }
    else{
        console_put_char_at(con, ' ', def_console->curr_x, def_console->curr_y);
        con->curr_x = con->width - 1;
    }
    con->curr_x++;
    
    if(con->curr_x == con->width){
        con->curr_x = 0;
        con->curr_y++;

        if(con->curr_y == con->height){
            con->curr_y--;
            for(y = 0; y < con->height - 1; y++){
                for(x = 0; x < con->width; x++){
                    con->buffer[(y * con->width) + x] = con->buffer[((y + 1) * con->width) + x];
                }
            }
            for(x = 0; x < con->width; x++){
                con->buffer[((con->height - 1) * con->width) + x] = ' ';
            }
            draw_console(con);
        }
    }
    gui_update_carret();
}

void console_clear_last_char(console *con){
    console_put_char_at(con, ' ', con->curr_x, con->curr_y);
    con->curr_x--;
    gui_update_carret();
}