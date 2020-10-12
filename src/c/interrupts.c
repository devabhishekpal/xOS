#include "kernel.h"

idtr IDTR;
x86_interrupt IDT[256];     //(x86_interrupt *)0xD00
byte pic1_irq_mask;
byte pic2_irq_mask;

void interrupt_default_handler(unsigned long route_code, struct interrupt_stack stack);

void init_interrupts(void){
    remap_pic(0x20, 0x28);  //Remaps the PICs so that IRQ0 starts at 0x20 and IRQ8 at 0x28
    mask_irq(ALL);
    
    //Using PIT 8253/8254
    int divisor = 1193180 / 100;    //1193180 is fixed frequency of input clock i.e 1.19Mhz. 100 is the value in Hz which will decide the data rate
    outportb(0x43, 0x36);   //Set the command byte to 0x36
    outportb(0x40, divisor & 0xFF); //low byte of divisor
    outportb(0x40, divisor >> 8);   //high byte of divisor
    //All the above functions are set with the standard for PIT and the values are defined in any PIT description

    load_exceptions();
    unmask_irq(TIMER);
    unmask_irq(KEYBOARD);
    add_int(0x20, int20, 0);
    add_int(0x21, int21, 0);
    add_int(0x30, int30, 0);
    load_idtr();

    asm("sti");
    asm("int %0" : : "i"(0x30));
}

void interrupt_default_handler(unsigned long route_code, struct interrupt_stack stack){
    if(route_code == 0x1001){
        timer_handler();
        pic_acknowledge(0x20);
    }
    else{
        switch (stack.eax){
        case 0xBADA:
            printf("Interrupt testing called\n");
            printf( "  eax: 0x%08X \tebx: 0x%08X \tecx: 0x%08X \tedx: 0x%08X\n", stack.eax, stack.ebx, stack.ecx, stack.edx );
			printf( "  esp: 0x%08X \tebp: 0x%08X \tesi: 0x%08X \tedi: 0x%08X\n", stack.esp, stack.ebp, stack.esi, stack.edi );
			printf( "  ds:  0x%04X \tes:  0x%04X \tfs:  0x%04X \tgs:  0x%04X\n", stack.ds, stack.es, stack.fs, stack.gs );
            printf("   eip: 0x%08x\n", stack.eip);
            break;
        case 0x1000:
            sys_break_handler(&stack);
            break;
        default:
            printf("Initialized: Interrupts!!!\n");
        }
        pic_acknowledge(48);
    }
}

void load_idtr(){
    IDTR.limit = 256*(sizeof(x86_interrupt)-1); //Set the maximum size of IDTR which is 2KB in 32bit protected mode (256 entries of 8bytes) and 4KB in 64bit (256 entries of 16bytes)
    IDTR.base = IDT;    //The base address of the IDT

    idtr *IDTRptr = &IDTR;  //Point to Interrupt Descriptor Table

    asm volatile("LIDT (%0) ": : "p"(IDTRptr));
}

void add_int(int number, void (*handler)(), dword dpl){
    word selector = 0x08;   //per-word-bits = 8bits
    word settings;
    dword offset = (dword)handler;

    //Get chip select
    asm volatile("movw %%cs,%0" :"=g"(selector));

    //Set settings from Descriptor Privilege Level (ring level)
    switch(dpl){
        case 0:
            settings = INT_0;
            break;
        case 1:
        case 2:
        case 3:
            settings = INT_3;
            break;
    }

    //Set the real values
    IDT[number].low_offset = (offset & 0xFFFF); //FFFF = 1111111111111111 for a gate the rearrangement is offset [0...15] goes to [0...15] so normal &ing will give the values
    IDT[number].selector = selector;
    IDT[number].settings = settings;
    IDT[number].high_offset = (offset >> 16);   //Higher limit shift 16bits right arithmetically as offset[16...31] are rearranged to [48...63]
}

void load_exceptions(){
    //Add all exceptions
    add_int(0, int00, 0);
    add_int(1, int01, 0);
    add_int(2, int02, 0);
    add_int(3, int03, 0);
    add_int(4, int04, 0);
    add_int(5, int05, 0);
    add_int(6, int06, 0);
    add_int(7, int07, 0);
    add_int(8, int08, 0);
    add_int(9, int09, 0);
    add_int(10, int10, 0);
    add_int(11, int11, 0);
    add_int(12, int12, 0);
    add_int(13, int13, 0);
    add_int(14, int14, 0);
    add_int(16, int16, 0);
    add_int(17, int17, 0);
    add_int(18, int18, 0);
    add_int(19, int19, 0);
    //External handlers from here
	add_int(20, 0, 0);
	//add_int(21, 0, 0);
	add_int(22, 0, 0);
	add_int(23, 0, 0);
	add_int(24, 0, 0);
	add_int(25, 0, 0);
	add_int(26, 0, 0);
	add_int(27, 0, 0);
	add_int(28, 0, 0);
	add_int(29, 0, 0);
	add_int(30, 0, 0);
	add_int(31, 0, 0);    

}

//Enable/Disable software interrupts
void switch_interrupt_flag(bool on){
    if(on){
        asm("sti"); //Set interrupt flag
    }
    else{
        asm("cli"); //Clear interrupt flag
    }
}

void pic_acknowledge(unsigned int interrupt){
    if(interrupt < 0x28){
        outportb(0x20, 0x20);   //PIC 1
    }
    else{
        outportb(0xA0, 0x20);   //PIC 2
    }
}

void panic(char *message, char *code, bool halt){
    printf("<SYSTEM ERRRRRRRRRROR> Panic!!!\n");

    if(halt == true){
        printf("A Fatal ");
    }
    if(halt == false){
        printf("A Non Fatal ");
    }
    printf("Exception has occurred: %s : %s", message, code);
    if(halt == true){
        printf("\nReally panic!!! System has been HALTED");
        asm("cli\n");   //Clear interrupt flag
        asm("hlt\n");   //Stop the system
    }
    else if(halt == false){
        //asm("hlt");
    }
    outportb(MASTER, EOI); //Send the End of Interrupt command to PIC
}

//Exception handlers here 00 - 19
void int_00(void){
	panic("Divide By Zero Error","#00", false);
}

void int_01(void){
	panic("Debug Error","#DB", false);
}

void int_02(void){
	panic("NMI Interrupt","#--", false);
}

void int_03(void){
	panic("Breakpoint","#BP", false);
}

void int_04(void){
	panic("Overflow","#OF", false);
}

void int_05(void){
	panic("BOUND Range Exceeded","#BR", false);
}

void int_06(void){
	panic("Invalid Opcode","#UD", false);
}

void int_07(void){
	panic("Device Not Available","#NM", false);
}

void int_08(void){
	panic("Double Fault","#DF", true);
}

void int_09(void){
	panic("Coprocessor Segment Overrun", "#NA", false);
}

void int_10(void){
	panic("Invalid TSS","#TS", false);
}

void int_11(void){
	panic("Segment Not Present","#NP", false);
}

void int_12(void){
	panic("Stack Segment Fault","#SS", false);
}

void int_13(void){
	//panic("Gneral Protection Fault","#GP", false);
}

void int_14(void){
	panic("Page Fault","#PF", false);
}

void int_16(void){
	panic("FPU Floating-Point Error","#MF", false);
}

void int_17(void){
	panic("Alignment Check","#AC", false);
}

void int_18(void){
	panic("Machine Check","#MC", true);
}

void int_19(void){
	panic("SIMD Floating-Point","#XF", false);
}

void remap_pic(int pic1, int pic2){
    byte md, sd;

    md = inportb(MASTERDATA);   //Save MASTER DATA state
    sd = inportb(SLAVEDATA);    //Save SLAVE DATA state

    outportb(MASTER, EOI);      //Send EOI signal, reset the chip

    outportb(MASTER, ICW1_INIT+ICW1_ICW4);  //Setup ICW1
    outportb(SLAVE, ICW1_INIT+ICW1_ICW4);

    outportb(MASTERDATA, pic1); //ICW2 is responsible for mapping IRQ 0-7
    outportb(SLAVEDATA, pic2); //for IRQ 8 -15

    //ICW3
    outportb(MASTERDATA, 0x04); 
    outportb(SLAVEDATA, 0x02);

    //ICW4
    outportb(MASTERDATA, ICW4_8086);
    outportb(SLAVEDATA, ICW4_8086);

    outportb(MASTERDATA, md);   //restore MASTER DATA
    outportb(SLAVEDATA, sd);    //restore SLAVE DATA
}

//Masks IRQ to prevent it from firing interrupt
void mask_irq(byte irq){
    if(irq == ALL){
        outportb(MASTERDATA, 0xFF); //All 1s
        outportb(SLAVEDATA, 0xFF);  //All 1s
        pic1_irq_mask = 0xFF;
        pic2_irq_mask = 0xFF;
    }
    else{
        irq = irq | (1<<irq);   //Check if 2^irq or irq is true
        if(irq < 8){
            outportb(MASTERDATA, irq&0xFF);
        }
        else{
            outportb(SLAVEDATA, irq>>8);
        }
    }
}

//Unmasks IRQ to allow firing interrupt
void unmask_irq(byte irq){
    if(irq == ALL){
        outportb(MASTERDATA, 0x00);
        outportb(SLAVEDATA, 0x00);
        pic1_irq_mask = 0x00;
        pic2_irq_mask = 0x00;
    }
    else{
        if(irq < 8){
            pic1_irq_mask = pic1_irq_mask ^ (1<<irq);
            outportb(MASTERDATA, pic1_irq_mask);
        }
        else{
            pic2_irq_mask = pic2_irq_mask ^ (1<<irq);
            outportb(SLAVEDATA, pic2_irq_mask);
        }
    }
}