#include "kernel.h"

#define PROCESS_DEBUG 0
#define PROCESS_DEBUG_LENGTH 10

void task_a(void);
void task_b(void);
void task_c(void);

void test_ab_a(void);
void test_ab_b(void);

struct process processes[MAX_PROCESSES];

int current_process;

//Setup initial processes
void init_processes(void){
    current_process = 0;
    printf("Initialize Processes!!!\n");
}

void start_processes(void){
    load_processes(task_a);
    load_processes(task_b);
    load_processes(task_c);

    task_a();
}

void start_ab_test(void){
    load_processes(test_ab_a);
    load_processes(test_ab_b);

    test_ab_a();
}

//Set processes to activate and initialize
void load_processes(void *process_main){
    int x;
    for(x = 0; x < MAX_PROCESSES; x++){
        if(processes[x].active == 0){
            processes[x].active = 1;
            processes[x].state.eip = (unsigned long)process_main; //Load address to instruction pointer
            break;
        }
    }
    printf("Loading process: %d starting at %08X\n", x, processes[x].state.eip);
}

//Find next process
unsigned int get_next_process(void){
    int x;
    bool have_process = false;

    for(x = current_process + 1; ((x < MAX_PROCESSES)&&(have_process == false)); x++){
        if(processes[x].active == 1){
            have_process = true;
            break;
        }
    }

    if(have_process == false){
        for(x = 0; ((x < current_process)&&(have_process == false)); x++){
            if(processes[x].active == 1){
                have_process = true;
                break;
            }
        }
    }

    if(have_process == false){
        printf("Cant find any such process...... Haltinggggggggg!!!!!!!!\n");
        while (1){
            ;   //Infinite loop to halt system
        }
    }

    return x;
}

//Handle system break

void sys_break_handler(struct interrupt_stack *stack){
    unsigned int next_process;

    if(PROCESS_DEBUG){
        printf("::Process Testing Called::\n");
        printf( "  eax:  0x%08X  ebx:  0x%08X  ecx:  0x%08X  edx:  0x%08X\n", stack->eax, stack->ebx, stack->ecx, stack->edx );
		printf( "  esp:  0x%08X  ebp:  0x%08X  esi:  0x%08X  edi:  0x%08X\n", stack->esp, stack->ebp, stack->esi, stack->edi );
		printf( "  ds:   0x%04X  es:   0x%04X  fs:   0x%04X  gs:   0x%04X\n", stack->ds, stack->es, stack->fs, stack->gs );
		printf( "  eip:  0x%08X\n", stack->eip );
    }
    next_process = get_next_process();

    if(PROCESS_DEBUG){
        printf("Current Process: %d\n", current_process);
        printf("Next Process: %d\n", next_process);
    }

    processes[current_process].state.eax = stack->eax;
    processes[current_process].state.ebx = stack->ebx;
	processes[current_process].state.ecx = stack->ecx;
	processes[current_process].state.edx = stack->edx;
	processes[current_process].state.esp = stack->esp;
	processes[current_process].state.ebp = stack->ebp;
	processes[current_process].state.esi = stack->esi;
	processes[current_process].state.edi = stack->edi;
	processes[current_process].state.ds = stack->ds;
	processes[current_process].state.es = stack->es;
	processes[current_process].state.fs = stack->fs;
	processes[current_process].state.gs = stack->gs;
	processes[current_process].state.eip = stack->eip;

    current_process = next_process;

    stack->eax = processes[current_process].state.eax;
	stack->ebx = processes[current_process].state.ebx;
	stack->ecx = processes[current_process].state.ecx;
	stack->edx = processes[current_process].state.edx;
	stack->esp = processes[current_process].state.esp;
	stack->ebp = processes[current_process].state.ebp;
	stack->esi = processes[current_process].state.esi;
	stack->edi = processes[current_process].state.edi;
	stack->ds = processes[current_process].state.ds;
	stack->es = processes[current_process].state.es;
	stack->fs = processes[current_process].state.fs;
	stack->gs = processes[current_process].state.gs;
	stack->eip = processes[current_process].state.eip;
}


//These use the classic race condition code to test mem leak one task pre-empting into another
void test_ab_a(void){
    long x = 0;
    while (1)
    {
        printf("A");
        while (x < 2000000){
            x++;
        }
        x = 0;
        sys_break();   
    }
}

void test_ab_b(void){
    long x = 0;
    while (1)
    {
        printf("B");
        while (x < 2000000){
            x++;
        }
        x = 0;
        sys_break();   
    }
}

void task_a(void){
    int x = 0;
    while (x < PROCESS_DEBUG_LENGTH){
        printf("A");
        x++;
        sys_break();
    }
    while(1){
    }
}

void task_b(void){
    int x = 0;
    while (x < PROCESS_DEBUG_LENGTH){
        printf("B");
        x++;
        sys_break();
    }
    while(1){
    }
}

void task_c(void){
    int x = 0;
    while (x < PROCESS_DEBUG_LENGTH){
        printf("C");
        x++;
        sys_break();
    }
    while(1){
    }
}