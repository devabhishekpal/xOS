#include "kernel.h"
#include "tamzen.h"

unsigned int _charac_width;
unsigned int _charac_height;
unsigned int _screen_border;
unsigned int _fg_color;
unsigned int _bg_color;
bool render_bg;

bool _is_vga_active = false;

uint8_t *prev_mouse_data;
unsigned int prev_mouse_x;
unsigned int prev_mouse_y;
bool is_first_time_drawn;


vga_info _vga_info;

void init_vga(void){
       multiboot_info_t *multiboot_info = get_multiboot_header();

       _vga_info.pitch = (unsigned int)multiboot_info->framebuffer_pitch;
       _vga_info.width = (unsigned int)multiboot_info->framebuffer_width;
       _vga_info.height = (unsigned int)multiboot_info->framebuffer_height;
       _vga_info.bpp = (unsigned int)multiboot_info->framebuffer_bpp;
       _vga_info.pixel_width = _vga_info.pitch / _vga_info.width;
       _vga_info.fbuffer = (unsigned char *)multiboot_info->framebuffer_addr;

       _vga_info.char_height = 14;
       _vga_info.char_width = 7;
       _vga_info.fg_color = 0x000000;
       _vga_info.bg_color = 0xFFFFFF;
       
       _charac_height = 14;  //All characters are 8x8 pixels so 8bits used
       _charac_width = 7;
       _screen_border = 10;

       _bg_color = 0xFFFFFF;
       _fg_color = 0x000000;
       render_bg = true;

       prev_mouse_data = malloc(sizeof(uint8_t) + (14 * 3 * 4 * 8));
       is_first_time_drawn = true;

       fillrect(_vga_info.fbuffer, _bg_color, 0, 0, 1280, 720);
       _is_vga_active = true;
}

bool is_vga_active(void){
       return _is_vga_active;
}

vga_info *get_vga_info(void){
       return &_vga_info;
}

inline void move_rect(rect dest, rect src){
       unsigned int i = 0;
       unsigned char *mem_dest;
       unsigned char *mem_src;
       unsigned int mem_size;

       for(i = 0; i < src.h; i++){
              mem_dest = _vga_info.fbuffer + (dest.x * _vga_info.pixel_width) + ((dest.y + i) * _vga_info.pitch);
              mem_src = _vga_info.fbuffer + (src.x * _vga_info.pixel_width) + ((src.y + i) * _vga_info.pitch);
              mem_size = 3 * (_vga_info.pixel_width * src.w);

              for(; mem_size != 0; mem_size --){
                     *mem_dest++ = *mem_src++;   //Allocate the same size as destination to src
              }
       }
}

void vga_move_line(unsigned int dest_y, unsigned int src_y){
       rect src;
       rect dest;

       src.x = 0;    //Dont change position horizontally
       src.y = _screen_border + (src_y * _charac_height);
       src.w = _vga_info.width;
       src.h = _charac_height;     //Line height is the height of character

       dest.x = 0;
       dest.y = _screen_border + (dest_y * _charac_height);
       dest.w = _vga_info.width;
       dest.h = _charac_height;

       move_rect(dest, src);
}

void vga_put_char(unsigned char c, unsigned int x, unsigned int y){
       draw_char(_vga_info.fbuffer, _screen_border + (x * _charac_width), _screen_border + (y * _charac_height), _vga_info.fg_color, _vga_info.bg_color, c);
}

inline void putpixel(uint8_t *buffer, unsigned int x, unsigned int y, int color){
       unsigned int pos = (x * _vga_info.pixel_width) + (y * _vga_info.width * _vga_info.pixel_width);

       /*
       * RGB in 15bit mode is 0 xxxxx xxxxx xxxxx
       *                        ----- ----- -----
       *                          R     G     B
       * 255 in binary is 11111111
       * Now for example for red the hex is ff0000
       * ff0000 = 111111110000000000000000
       * shifting this right by 16bits using arithmetic shift right gives 11111111 for R value
       * shifting this right by 8bits using arithmetic shift right gives 1111111100000000 for G values
       * and for B it is the same as no bits are shifted
       * 
       * Now &ing with 255 for each we get
       * R = 11111111 & 11111111 = 11111111(0d255)
       * G = 1111111100000000 & 11111111 = (0d000)
       * B = 111111110000000000000000 & 11111111 = (0d000)
       * 
       * which infact is the binary representation of red in RGB i.e RGB(255, 0, 0)
       * 
       * Taking another example let us take yellow with a hex of fff700
       * fff700 = 111111111111011100000000
       * R = (111111111111011100000000 >> 16) = 11111111
       * G = (111111111111011100000000 >> 8) = 1111111111110111
       * B = (111111111111011100000000)
       * 
       * &ing with 255 (0b11111111)
       * 
       * R = (11111111 & 11111111) = 11111111 (0d255)
       * G = (1111111111110111 & 11111111) = 11110111 (0d247)
       * B = (111111111111011100000000 & 11111111) = 00000000 (0d000)
       * 
       * which infact we again see is yellow (#fff700) in RGB(255,247,0)
       */
       buffer[pos] = color & 255;   //B
       buffer[pos+1] = (color>>8) & 255; //G
       buffer[pos+2] = (color>>16) & 255; //R
}

void draw_rect(rect r, unsigned int color){
       fillrect(_vga_info.fbuffer, color, r.x, r.y, r.w, r.h);
}

inline void fillrect(uint8_t *buffer, uint32_t color, unsigned int x, unsigned int y, unsigned int w, unsigned int h){
       uint8_t *pos = (buffer + (x * _vga_info.pixel_width) + (y * _vga_info.width * _vga_info.pixel_width));
       unsigned int i,j;

       for(i = 0; i < h; i++){
              for(j = 0; j < w; j++){
                     *(uint32_t *)(pos + (j * _vga_info.pixel_width)) = color;
              }
              pos += _vga_info.pitch;
       }
}

void draw_string(char *string, unsigned int x, unsigned int y, unsigned int fg_color, unsigned int bg_color){
       unsigned int offset;
       offset = 0;
       while (*string){
              draw_char(_vga_info.fbuffer, x, y, fg_color, bg_color, *string);
              x += 7;
              string++;
       }      
}

inline void draw_char(uint8_t *buffer, unsigned int x, unsigned int y, uint32_t fg, uint32_t bg, unsigned int font_char){
       int row, col;
       uint8_t *pos = (buffer + (x * _vga_info.pixel_width) + (y * _vga_info.width * _vga_info.pixel_width));

       for(row = 0; row < 14; row++){
              for(col = 7; col > -1; col--){
                     if((_tamzen_bits[_tamzen_offset[font_char] + row]>>8 & (1<<col))){
                            *(uint32_t *)(pos + (7 - col) * _vga_info.pixel_width) = fg;
                     }
                     else{
                            if(render_bg){
                                   *(uint32_t *)(pos + (7 - col) * _vga_info.pixel_width) = bg;
                            }
                     }
              }
              pos += _vga_info.pitch;
       }
}

void draw_mouse(unsigned int x, unsigned int y){
       unsigned int i = 0;
       int row, col;
       uint8_t *mem_dest;
       uint8_t *mem_src;
       unsigned int mem_size;
       uint8_t *pos = (_vga_info.fbuffer + (x * _vga_info.pixel_width) + (y * _vga_info.width * _vga_info.pixel_width));

       //Change buffer to what it was before placement
       if(!is_first_time_drawn){
              for(i = 0; i < 14; i++){
                     mem_dest = _vga_info.fbuffer + (prev_mouse_x * _vga_info.pixel_width) + ((prev_mouse_y + 1) * _vga_info.pitch);
                     mem_src = prev_mouse_data + (i * (8 * _vga_info.pixel_width));
                     mem_size = (_vga_info.pixel_width * 8);

                     for(; mem_size != 0; mem_size--){
                            *mem_dest++ = *mem_src++;
                     }
              }
       }

       //Take buffer value to new place and save it
       for(i = 0; i< 14; i++){
              mem_src = _vga_info.fbuffer + (x * _vga_info.pixel_width) + ((y + i) * _vga_info.pitch);
              mem_dest = prev_mouse_data + (i * (8 * _vga_info.pixel_width));
              mem_size = (_vga_info.pixel_width * 8);

              for(; mem_size != 0; mem_size--){
                     *mem_dest++ = *mem_src++;
              }
       }

       draw_char(_vga_info.fbuffer, x, y, color_xo_white, color_black, '*');

       //Save data for future
       prev_mouse_x = x;
       prev_mouse_y = y;
       is_first_time_drawn = false;
}