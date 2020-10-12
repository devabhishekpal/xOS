#define color_titlebar 0x000000
#define color_titlebar_text 0xFFFFFF
#define color_window 0xFFFFFF
#define color_window_box 0x000000
#define color_cntrl_background 0xFFFFFF
#define color_cntrl_text 0x000000

/*
  -------------------------------------------------------   1px border
 | 25px Titlebar                                         |
  -------------------------------------------------------   
 |                                                       |
 |                                                       |
 |                                                       |
 |                                                       |
 |                                                       |
  ------------------------------------------------------- 
*/

struct _window{
    rect box;
    char title[100];
    unsigned int fg_color;
    unsigned int bg_color;
};
typedef struct _window window;

struct _console{
    rect box;
    unsigned int width;
    unsigned int height;
    unsigned int fg_color;
    unsigned int bg_color;
    char *buffer;
    unsigned int curr_x;
    unsigned int curr_y;
    unsigned int prev_x;
    unsigned int prev_y;
    bool waiting_on_inp;
};
typedef struct _console console;

void init_gui(void);
void draw_base(void);
bool is_gui_active(void);
void gui_update_carret(void);
void gui_move_row(unsigned int dest, unsigned int src);
void gui_put_char_at(char c, unsigned int x, unsigned int y);
void gui_write_string_at(const char *data, unsigned int x, unsigned int y);
void gui_flip_cursor(void);
void gui_set_default_console_curr_row_col(unsigned int row, unsigned int col);
void gui_put_char(char c);
void gui_clear_last_char(void);

window *window_new(char *title, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
void draw_window(window *win);

void console_set_def(console *con);
console *console_new(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int bg_color, unsigned int fg_color);
void draw_console(console *con);
void console_put_char_at(console *con, char c, unsigned int x, unsigned int y);
void console_put_char(console *con, char c);
void console_clear_last_char(console *con);
