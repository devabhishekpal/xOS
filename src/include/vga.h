#define red 0xFF0000
#define green 0x00FF00
#define blue 0x0000FF
#define white 0xFFFFFF
#define black 0x000000
#define xo_white 0xFFFFFF

struct _rect{
    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;
};
typedef struct _rect rect;

struct __vga_info{
    unsigned char *fbuffer;
    unsigned int pitch;
    unsigned int width;
    unsigned int height;
    unsigned int bpp;
    unsigned int pixel_width;
};
typedef struct __vga_info vga_info;

void init_vga(void);
bool is_vga_active(void);
vga_info * get_vga_info(void);
void putpixel(unsigned char* screen, unsigned int x,unsigned int y, int color) __attribute__((always_inline));
void fillrect(unsigned char *vram, unsigned int color, unsigned int x, unsigned int y, unsigned int w, unsigned int h) __attribute__((always_inline));
void vga_put_char(unsigned char c, unsigned int x, unsigned int y);
void vga_move_line(unsigned int dest_y, unsigned int src_y);
void move_rect(rect dest, rect src) __attribute__((always_inline));
void draw_char(unsigned char * screen, unsigned int x, unsigned int y, unsigned int color, unsigned int font_char) __attribute__((always_inline));
void draw_string(char * string, unsigned int x, unsigned int y, unsigned int color);
