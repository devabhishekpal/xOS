//Basic OS requirements like printf()


#define GET_DEFINE_STRING(x)
#define STRING_VALUE_OF(x) GET_DEFINE_STRING(x)

struct node{
    void *data;
    struct node *next;
};

void list_push(struct node** head_ref, void *new_data, size_t data_size);
int list_for_each(struct node* head_ref, int (*func)(struct node* n));

//Output byte to specified hardware port
static inline void outportb(dword port, byte value){
    __asm__ __volatile__ ("outb %%al,%%dx"::"d"(port), "a" (value));
}

//Output word to port
static inline void outportw(dword port, dword value){
    __asm__ __volatile__ ("outw %%ax,%%dx"::"d"(port), "a" (value));
}

//Get a byte from port
static inline byte inportb(dword port){
    byte value;
    __asm__ __volatile__ ("inb %w1,%b0" : "=a"(value) : "d"(port));
    return value;
}

void get_string(char* string, unsigned int size);
void putc(void *p, char c);
size_t strlen(const char* str);
void update_cursor(int row, int col);

void init_printf(void* putp,void (*putf) (void*,char));
void tfp_printf(char *fmt, ...);
void tfp_sprintf(char* s,char *fmt, ...);
void tfp_format(void* putp,void (*putf) (void*,char),char *fmt, va_list va);

#define printf tfp_printf 
#define sprintf tfp_sprintf 

void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *dest, char val, size_t count);
char *strcpy(char *dest, const char *src);
int strcmp (const char *s1, const char *s2);
void printfcomma2 (int n);
void printfcomma (int n);