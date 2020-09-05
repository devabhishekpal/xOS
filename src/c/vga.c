#include "kernel.h"
#include "tamzen.h"

unsigned int _charac_width;
unsigned int _charac_height;
unsigned int _screen_border;
unsigned int _fg_color;
unsigned int _bg_color;
bool render_bg;

bool _is_vga_active = false;
vga_info _vga_info;

void init_vga(void){
       multiboot_info_t *multiboot_info = get_multiboot_header();

       _vga_info.pitch = (unsigned int)multiboot_info->framebuffer_pitch;
       _vga_info.width = (unsigned int)multiboot_info->framebuffer_width;
       _vga_info.height = (unsigned int)multiboot_info->framebuffer_height;
       _vga_info.bpp = (unsigned int)multiboot_info->framebuffer_bpp;
       _vga_info.pixel_width = _vga_info.pitch / _vga_info.width;
       _vga_info.fbuffer = (unsigned char *)multiboot_info->framebuffer_addr;

       _charac_height = 14;  //All characters are 8x8 pixels so 8bits used
       _charac_width = 7;
       _screen_border = 10;

       _bg_color = xo_white;
       _fg_color = black;
       render_bg = true;

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
       draw_char(_vga_info.fbuffer, _screen_border + (x * _charac_width), _screen_border + (y * _charac_height), black, c);
}

inline void putpixel(unsigned char *screen, unsigned int x, unsigned int y, int color){
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
       screen[pos] = color & 255;   //B
       screen[pos+1] = (color>>8) & 255; //G
       screen[pos+2] = (color>>16) & 255; //R
}

inline void fillrect(unsigned char *vram, unsigned int color, unsigned int x, unsigned int y, unsigned int w, unsigned int h){
       unsigned char *pos = vram;
       unsigned int offset = (x * _vga_info.pixel_width) + (y * _vga_info.width * _vga_info.pixel_width);
       int i,j;

       for(i = 0; i < h; i++){
              for(j = 0; j < w; j++){
                     pos[offset + (j * _vga_info.pixel_width)] = color & 255;  //B
                     pos[offset + (j * _vga_info.pixel_width + 1)] = (color>>8) & 255;       //G
                     pos[offset + (j * _vga_info.pixel_width + 2)] = (color>>16) & 255;      //R
              }
              pos += _vga_info.pitch;
       }
}

void draw_string(char *string, unsigned int x, unsigned int y, unsigned int color){
       unsigned int offset;
       offset = 0;
       while (*string){
              draw_char(_vga_info.fbuffer, x + offset * 8, y, color, *string);

              offset++;
              string++;
       }      
}

inline void draw_char(unsigned char *screen, unsigned int x, unsigned int y, unsigned int color, unsigned int font_char){
       int row;
       int col;
       /*
       for(row = 0; row < 8; row++){
              for(col = 8; col > -1; col--){
                     if(font_char[row] & (1<<col)){
                            putpixel(screen, x + col, y + row, color);
                     }
                     else{
                            if(render_bg){
                                   putpixel(screen, x + col, y + row, _bg_color);
                            }
                     }
              }
       }*/

       /* Alternative for a long font */
       unsigned char font_bits;

       for (row = 0; row < 14; row++) {
    	       if(font_char == 255){
                     font_bits = char_block[0 + row] >> 8;
              }
              else{
                     font_bits = _tamzen_bits[_tamzen_offset[font_char] + row] >> 8;
              }
              for (col = 7; col > -1; col--) {
                     if((font_bits & (1 << col))) {
                     putpixel(screen, x + 7 - col, y + row, color);
                     }
                     else {
                            if(render_bg) {
                                   putpixel(screen, x + 7 - col, y + row, _bg_color);
                            }
                     }
              }
       }
}