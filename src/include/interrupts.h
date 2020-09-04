
#define INT_0 0x8E00    //1000111000000000 = present, ring0, int gate
#define INT_3 0xEE00    //1110111000000000 = present, ring3, int gate

//Interrupt Request macros
#define ALL 0xFF
#define TIMER 0
#define KEYBOARD 1
#define CASCADE 2
#define COM2_4 3
#define COM1_3 4
#define LPT 5
#define FLOPPY 6
#define FREE7 7
#define CLOCK 8
#define FREE9 9
#define FREE10 10
#define FREE11 11
#define PS2MOUSE 12
#define COPROC 13
#define IDE_1 14
#define IDE_2 15

//PIC macros
//+--------------------------+
//|        |  PIC1  |  PIC2  |
//|--------|--------|--------|
//|COMMAND |  0x20  |  0xA0  |
//|DATA    |  0x21  |  0xA1  |
//+--------------------------+


//Master = PIC 1
#define MASTER 0x20 //COMMAND
#define MASTERDATA 0x21 //DATA
//Slave = PIC 2
#define SLAVE 0xA0  //COMMAND
#define SLAVEDATA 0xA1  //DATA
#define EOI 0x20

//Init Command Word 1 and Init Command Word 4 are the only ones we need to work on

//This is the layout of the Initialization Command Words 1 to 4

//ICW1
/*+-----------------------------------------------------------------------------------------------------------------------------+
* |     A0      |     D7      |     D6      |     D5      |     D4      |     D3      |     D2      |     D1      |     D0      |
* |-------------|-------------|-------------|-------------|-------------|-------------|-------------|-------------|-------------|
* |     0       |     A7      |     A6      |     A5      |      1      |    LTIM     |     ADI     |    SNGL     |     IC4     |
* +-----------------------------------------------------------------------------------------------------------------------------+
*D4 = 1 and  A0 = 0 means start ICW1
*A7-A5-> Interrupt vector of MCs in 80/85 mode
*LTIM -> Level(1) or Edge(0) triggered
*ADI -> Call Address Interval 4bytes(1) or 8bytes(0)
*SNGL -> Single(1) or Cascaded(0)
*IC4 -> Needs ICW4 or not*/


//ICW2
/*+-----------------------------------------------------------------------------------------------------------------------------+
* |     A0      |     D7      |     D6      |     D5      |     D4      |     D3      |     D2      |     D1      |     D0      |
* |-------------|-------------|-------------|-------------|-------------|-------------|-------------|-------------|-------------|
* |     1       |     T7      |     T6      |     T5      |     T4      |     T3      |     A10     |     A9      |      A8     |
* +-----------------------------------------------------------------------------------------------------------------------------+
*A0 = 1 means select ICW2 
*T7-T3 -> A3-A0 of interrupt address
*A10,A9,A8 -> Selected according to interrupt request level
**/


//ICW3
/*+-----------------------------------------------------------------------------------------------------------------------------+
* |     A0      |     D7      |     D6      |     D5      |     D4      |     D3      |     D2      |     D1      |     D0      |
* |-------------|-------------|-------------|-------------|-------------|-------------|-------------|-------------|-------------|
* |     1       |     S7      |     S6      |     S5      |      S4     |     S3      |     S2      |     S1      |      S0     |
* +-----------------------------------------------------------------------------------------------------------------------------+
* S(n) = 1 -> IR(n) has slave
* S(n) = 0 -> IR(n) doesnt has slave
* If S7 - S3 are 0s then it will be slave mode and S2-S0 will act as ID2-ID0
* D2-D1-D0 = 000 to 111 for IR0 to IR7 i.e slave 1 to 8
**/


//ICW4
/*+-----------------------------------------------------------------------------------------------------------------------------+
* |     A0      |     D7      |     D6      |     D5      |     D4      |     D3      |     D2      |     D1      |     D0      |
* |-------------|-------------|-------------|-------------|-------------|-------------|-------------|-------------|-------------|
* |      1      |      0      |      0      |      0      |    SFNM     |     BUF     |     M/S     |    AEOI     |     uPM     |
* +-----------------------------------------------------------------------------------------------------------------------------+
* SFNM -> Special Fully Nested Mode(1) Or Not Special Fully Nested Mode (0)
* BUF and M/S work together
* If BUF is disabled then it is non buffered and M/S is not activated
* If BUF = 1, then M/S = Slave(0) or Master(1)
* AEOI = Auto EOI(1) or Normal EOI(0)
* uPM = 8086/8088 Mode(1) or 80/85 mode(0)
*/

#define ICW1_INIT 0x10  //required for PIC initialization (10000) (D4 A0)
#define ICW1_EDGE 0x08 //Edge triggered IRQs (D3) (1000)
#define ICW1_SINGLE 0x02 //Single mode MASTER (No cascade)(10)
#define ICW1_ICW4 0x01  //Enable ICW4 (D0) (1)

//-----------------------------------------------------------
//Total = 10000 + 1000 + 10 + 1 = 11011 = 000011011 for ICW1
//-----------------------------------------------------------

#define ICW4_SFNM 0x10  //Special Fully Nested Mode (10000)
#define ICW4_BUFFER 0x08    //Buffered Mode (1000)
#define ICW4_MASTER 0x04 //This is the Master PIC (enable)(100)
#define ICW4_AEOI 0x02 //Auto EOI enabled(10)
#define ICW4_8086 0x01 //8086 mode (1)

//----------------------------------------------------------------
//Total = 10000 + 1000 + 100 +10 + 1 = 11111 = 100011111 for ICW4
//----------------------------------------------------------------
//Note the first bit is 1 as IC4 from ICW1 is 1 and hence ICW4 is selected and A0 is enableds

//Data structure for interrupt
typedef struct{
    word low_offset;    //low nibble offset for interrupt handler
    word selector;      //GDT selector to use
    word settings;      //Interrupt settings
    word high_offset    //high nibble offset to interrupt handler
} __attribute__ ((packed)) x86_interrupt;

//Data structure for Interrupt Descriptor Table
typedef struct{
    word limit; //Size of IDT
    x86_interrupt *base;    //pointer to base of IDT
} __attribute__ ((packed)) idtr;

extern void int00(void);
extern void int01(void);
extern void int02(void);
extern void int03(void);
extern void int04(void);
extern void int05(void);
extern void int06(void);
extern void int07(void);
extern void int08(void);
extern void int09(void);
extern void int10(void);
extern void int11(void);
extern void int12(void);
extern void int13(void);
extern void int14(void);
extern void int15(void);
extern void int16(void);
extern void int17(void);
extern void int18(void);
extern void int19(void);
extern void int30(void);
extern void int21(void);

void int_00(void);
void int_01(void);
void int_02(void);
void int_03(void);
void int_04(void);
void int_05(void);
void int_06(void);
void int_07(void);
void int_08(void);
void int_10(void);
void int_11(void);
void int_12(void);
void int_13(void);
void int_14(void);
void int_16(void);
void int_17(void);
void int_18(void);
void int_19(void);

void load_idtr(void);   //Loading IDTR and setting up IDT
void add_int(int number, void (*handler)(), dword dpl);     //Add interrupt
void load_exceptions();     //init exceptions
void panic(char *message, char *code, bool halt);       //Panic in execution
void remap_pic(int pic1, int pic2);     //Remap PIC
void mask_irq(byte irq);        //PIC IRQ masking
void unmask_irq(byte irq);      //PIC IRQ unmasking
void switch_interrupt_flag(bool on); //Set interrrupt Flag (STI) or Clear Interrupt Flag(CLI)
void pic_acknowledge(unsigned int interrupt);
void init_interrupts(void);

struct interrupt_stack {
	unsigned long gs;
	unsigned long fs;
	unsigned long es;
	unsigned long ds;
	unsigned long edi;
	unsigned long esi;
	unsigned long ebp;
	unsigned long esp;
	unsigned long ebx;
	unsigned long edx;
	unsigned long ecx;
	unsigned long eax;
	unsigned int ss;
	unsigned long useresp;
	unsigned int cs;
	unsigned long eip;
};
