#include "kernel.h"

void list_push(struct node** head_ref, void *new_data, size_t data_size){

    struct node* new_node = (struct node*)malloc(sizeof(struct node));

    new_node->data = malloc(data_size);
    new_node->next = (*head_ref);

    //copy contents of new_data to allocated mem
    unsigned int i;
    for(i = 0; i < data_size; i++){
        *(char*)(new_node->data + i) = *(char*)(new_data+i);
    }

    //Shift head to new_node
    (*head_ref) = new_node;
}

int list_for_each(struct node* head_ref, int (*func)(struct node* n)){
    int return_val = 0;

    struct node* list = head_ref;

    while (list != NULL){
        return_val = func(list);

        if(return_val == 0){
            list = list->next;
        }
        else{
            list = NULL;
        }
    }

    return return_val;
}

void get_string(char* string, unsigned int size){
    char scancode;
    char user_char;
    unsigned int x;

    scancode = get_scancode();
    x = 0;

    while( scancode != 0x1C && x != size - 1){
        if(scancode != 0){
            user_char = get_character( (unsigned int)scancode );
            switch (user_char){
                case 0:
                    break;
                case '\b':          //Backspace
                    if(x > 1){
                        term_clear_last_char();
                        string[x] = '\0';   //NULL
                        x--;    //Decrease index i.e point to previous character
                    }
                    break;
                default:
                    if(is_gui_active()){
                        gui_put_char_at_input(user_char);
                    }
                    else{
                        term_put_char(user_char);
                    }
                    string[x] = user_char;
                    x++;
                    break;
            }
        }
        scancode = get_scancode();
    }
    if(is_gui_active()){
        gui_reset_input();
    }
    string[x] = 0;
}

//putc wrapper for printf()
void putc(void *p, char c){
    void *file_to_write_into = p;

    file_to_write_into++;
    term_put_char(c);
}

//Calc length of null terminated string
size_t strlen(const char *str){
    size_t len = 0;

    while(str[len]){
        len++;
    }
    return len;
}

//Mouse cursor updates
void update_cursor(int row, int col){
    if(!is_gui_active()){   //Means there is no need of cursor as it isnt in GUI mode
        //The index in a linear memory is indx = [(y*width)+x]
        unsigned short pos = (row*80)+col;

        /*The following sends a command to index 14(0x0E) and 15(0x0F) of the
        * CRT control register to VGA controller and show there the cursor
        * should flash (blink)
        */
        //LOW port to VGA index register
        outportb(0x3D4, 0x0F);  //Address register
        outportb(0x3D5, (unsigned char)(pos&0xFF));//Data register

        //HIGH port to VGA index register
        outportb(0x3D4, 0x0E);
        outportb(0x3D5, (unsigned char)((pos>>8)&0xFF));
    }
}

//Tiny printf implementation based on http://www.sparetimelabs.com/tinyprintf/tinyprintf.php
typedef void (*putcf)(void*, char);
static putcf stdout_putf;
static void* stdout_putp;
static void ui2a(unsigned int num, unsigned int base, int uc,char * bf)
{
	int n=0;
	unsigned int d=1;
	while (num/d >= base)
	    d*=base;        
	while (d!=0) {
	    int dgt = num / d;
	    num%= d;
	    d/=base;
	    if (n || dgt>0 || d==0) {
	        *bf++ = dgt+(dgt<10 ? '0' : (uc ? 'A' : 'a')-10);
	        ++n;
	        }
	    }
	*bf=0;
}

static void i2a (int num, char * bf)
{
    if (num<0) {
        num=-num;
        *bf++ = '-';
        }
    ui2a(num,10,0,bf);
}

static int a2d(char ch)
{
    if (ch>='0' && ch<='9') 
        return ch-'0';
    else if (ch>='a' && ch<='f')
        return ch-'a'+10;
    else if (ch>='A' && ch<='F')
        return ch-'A'+10;
    else return -1;
}

static char a2i(char ch, char** src,int base,int* nump)
{
    char* p= *src;
    int num=0;
    int digit;
    while ((digit=a2d(ch))>=0) {
        if (digit>base) break;
        num=num*base+digit;
        ch=*p++;
        }
    *src=p;
    *nump=num;
    return ch;
}

static void putchw(void* putp,putcf putf,int n, char z, char* bf)
{
    char fc=z? '0' : ' ';
    char ch;
    char* p=bf;
    while (*p++ && n > 0)
        n--;
    while (n-- > 0) 
        putf(putp,fc);
    while ((ch= *bf++))
        putf(putp,ch);
}

void tfp_format(void* putp,putcf putf,char *fmt, va_list va)
{
    char bf[12];
    
    char ch;


    while ((ch=*(fmt++))) {
        if (ch!='%') 
            putf(putp,ch);
        else {
            char lz=0;
#ifdef  PRINTF_LONG_SUPPORT
            char lng=0;
#endif
            int w=0;
            ch=*(fmt++);
            if (ch=='0') {
                ch=*(fmt++);
                lz=1;
                }
            if (ch>='0' && ch<='9') {
                ch=a2i(ch,&fmt,10,&w);
                }
#ifdef  PRINTF_LONG_SUPPORT
            if (ch=='l') {
                ch=*(fmt++);
                lng=1;
            }
#endif
            switch (ch) {
                case 0: 
                    goto abort;
                case 'u' : {
#ifdef  PRINTF_LONG_SUPPORT
                    if (lng)
                        uli2a(va_arg(va, unsigned long int),10,0,bf);
                    else
#endif
                    ui2a(va_arg(va, unsigned int),10,0,bf);
                    putchw(putp,putf,w,lz,bf);
                    break;
                    }
                case 'd' :  {
#ifdef  PRINTF_LONG_SUPPORT
                    if (lng)
                        li2a(va_arg(va, unsigned long int),bf);
                    else
#endif
                    i2a(va_arg(va, int),bf);
                    putchw(putp,putf,w,lz,bf);
                    break;
                    }
                case 'x': case 'X' : 
#ifdef  PRINTF_LONG_SUPPORT
                    if (lng)
                        uli2a(va_arg(va, unsigned long int),16,(ch=='X'),bf);
                    else
#endif
                    ui2a(va_arg(va, unsigned int),16,(ch=='X'),bf);
                    putchw(putp,putf,w,lz,bf);
                    break;
                case 'c' : 
                    putf(putp,(char)(va_arg(va, int)));
                    break;
                case 's' : 
                    putchw(putp,putf,w,0,va_arg(va, char*));
                    break;
                case '%' :
                    putf(putp,ch);
                default:
                    break;
                }
            }
        }
    abort:;
}

void init_printf(void* putp,void (*putf) (void*,char))
{
    stdout_putf=putf;
    stdout_putp=putp;
}

void tfp_printf(char *fmt, ...)
{
    va_list va;
    va_start(va,fmt);
    tfp_format(stdout_putp,stdout_putf,fmt,va);
    va_end(va);
}

static void putcp(void* p,char c)
{
    *(*((char**)p))++ = c;
}

void tfp_sprintf(char* s,char *fmt, ...)
{
    va_list va;
    va_start(va,fmt);
    tfp_format(&s,putcp,fmt,va);
    putcp(&s,0);
    va_end(va);
}
 /* End tiny printf() implementation */

//Copt count bytes from src to dest
void *memcpy(void *dest, const void *src, size_t count){
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

//set count bytes of dest to val
void *memset(void *dest, char val, size_t count){
    unsigned char *temp = (unsigned char *)dest;
    for(;count != 0; count --, temp[count] = val); //Setting the value
    return dest;
}

char *strcpy(char *dest, const char *src){
    unsigned i;
    for(i = 0; src[i] != '\0'; ++i){
        dest[i] = src[i];
    }
    dest[i] = '\0'; //Setting last character as null
    return dest;
}

int strcmp (const char *s1, const char *s2){
    for(;*s1 == *s2; ++s1, ++s2){
        if(*s1 == 0){
            return 0;
        }
    }
    return *(unsigned char *)s1 < *(unsigned char *)s1 ? -1 : 1;
}

void printfcomma2(int n){
    if(n < 1000){
        printf("%d", n);
        return;
    }
    printfcomma2(n/1000);
    printf (",%03d", n%1000);
}

void printfcomma(int n){
    if(n < 0){
        printf("-");
        n = -n;
    }
    printfcomma2(n);
}