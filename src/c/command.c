#include "kernel.h"

struct node *command;

void init_commands(void){   //Initialize commands and the associated word
    add_command("start_processes", &cmd_start_processes);
    add_command("mem_leak_test", &cmd_process_ab_test);
    add_command("print_sizes", &cmd_print_sizes);
    add_command("show_commands", &cmd_list_commands);

    printf("Initialized the commands: \n");
}

void launch_commands_loop(void){        //Displays the commands present
    char command_string[255];

    while(1){
        strcpy(command_string, "");
        if(is_gui_active()){
            get_string(command_string, 255);
        }
        else{
            printf("xOS: ");
            get_string(command_string, 255);
            printf("\n");
        }
        if(iterate_over_commands(command_string) == 0){
            printf("No such command as \"%s\" present... use show_commands to view the commands.\n", command_string);
        }
    }
}

int iterate_over_commands(char *command_string){
    int ret_val = 0;

    struct node *list = command;

    while(list != NULL){
        struct command *current_command;
        current_command = (struct command*)list->data;

        //printf( "\ncs: \"%s\", trig: \"%s\", loc: 0x%08X\n", command_string, current_command->trigger, current_command->main_function );


        if(strcmp(current_command->trigger, command_string) == 0){
            void (*function)(void) = current_command->main_func;
            function();
            ret_val = 1;
            list = NULL;
        }
        else{
            list = list->next;
        }
    }
    return ret_val;
}

void add_command(char *_trigger, void(* _main_func)){
    struct command *new_command = malloc(sizeof(struct command));
    new_command->trigger = malloc(strlen(_trigger) + 1);
    strcpy(new_command->trigger, _trigger);
    new_command->main_func = _main_func;
    list_push(&command, new_command, sizeof(struct command));
}

void cmd_start_processes(void){
    start_processes();
}

void cmd_process_ab_test(void){
    start_ab_test();
}

void cmd_print_sizes(void){
    printf("size of ushort: %d\n", sizeof(unsigned short));
    printf("size of ulong: %d\n", sizeof(unsigned long));
}

void cmd_list_commands(void){
    list_for_each(command, &cmd_list_commands_print);
    printf("\n");
}

int cmd_list_commands_print(struct node *n){
    struct command *comm = (struct command *)n->data;
    printf("%s\n", comm->trigger);
    return 0;
}