#include "kernel.h"

void *mem_top;

void init_memory(void){
    mem_top = (void *)0x400000; //Allocate 4194304 bytes = 4GB
}

void *get_mem_top(void){
    return mem_top;
}

void *malloc(long size){
    void *return_val = mem_top;
    mem_top = mem_top+size;
    return return_val;
}