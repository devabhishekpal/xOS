#include "kernel.h"

struct node *command;

void init_commands(void){   //Initialize commands and the associated word
    add_command("start_processes", &cmd_start_processes, "Begins primary process test");
    add_command("mem_leak_test", &cmd_process_ab_test, "Spits AB to screen pre-emptively to test meory leaks");
    add_command("print_sizes", &cmd_print_sizes, "Displays the sizes of the common data types");
    add_command("disp_input", &cmd_disp_inp, "Displays the pressed scancode");
    add_command("!", &cmd_list_commands, "Shows the available commands");
    add_command("test_mouse", &cmd_test_mouse, "Test the mouse");
    printf("Initialized: Commands!!!\n");
}

void launch_commands_loop(void){        //Displays the commands present
    char command_string[255];

    while(1){
        strcpy(command_string, "");
        printf("xOS: ");
        get_string(command_string, 255);

        if(iterate_over_commands(command_string) == 0){
            printf("No such command as \"%s\" present... use \'!\' to view the commands.\n", command_string);
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

void add_command(char *_trigger, void(*_main_func), char *_desc){
    struct command *new_command = malloc(sizeof(struct command));
    new_command->trigger = malloc(strlen(_trigger) + 1);
    strcpy(new_command->trigger, _trigger);

    new_command->desc = malloc(strlen(_desc) + 1);
    strcpy(new_command->desc, _desc);

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
    printf("\n");
    printf("ushort:     %d\n", sizeof(unsigned short));
    printf("ulong:      %d\n", sizeof(unsigned long));
    printf("uint32_t:   %d\n", sizeof(uint32_t));
    printf("\n");
}

void cmd_list_commands(void){
    printf("\nCommands List:\n\n");
    list_for_each(command, &cmd_list_commands_print);
    printf("\n");
}

int cmd_list_commands_print(struct node *n){
    struct command *comm = (struct command *)n->data;
    printf("%15s  ->  %s\n", comm->trigger, comm->desc);
    return 0;
}

void cmd_disp_inp(void){
    char scancode;
    printf("Press key for scancode readout. Press ESC to exit.\n");
    scancode = get_scancode();
    while(scancode != 0x01){
        if(scancode != 0){
            printf("Key:    0x%02X %d\n", scancode, scancode);
        }
        scancode = get_scancode();
    }

    printf("Done finding scancode.\n");
}

void cmd_test_mouse(void){
    uint32_t x = 0;
    uint32_t y = 0;
    char scancode;

    draw_mouse(1280 / 2, 720 / 2);
    printf("Use arrow keys to control mouse. Press ESC to quit.\n");

    scancode = get_scancode();

    while(scancode != 0x01){
        if(scancode == 0x4D){
            x++;
            draw_mouse(1280 / 2 + x, 720 / 2 +y);
        }
        if(scancode == 0x4B){   //Left key i.e move left
            x--;
            draw_mouse(1280 / 2 + x, 720 / 2 + y);
        }
        if(scancode == 0x48){
            y--;
            draw_mouse(1280 / 2 + x, 720 / 2 + y);
        }
        if(scancode == 0x50){
            y++;
            draw_mouse(1280 / 2 + x, 720 / 2 + y);
        }
        scancode = get_scancode();
    }
    printf("Mouse testing complete\n");
}