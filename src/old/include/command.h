struct command{
    char *trigger;
    void (*main_func)(void);
    char *desc;
};

void init_commands(void);
void launch_commands_loop(void);
int iterate_over_commands(char *command_string);
void add_command(char *trigger, void *main_func, char *_desc);
void cmd_start_processes(void);
void cmd_process_ab_test(void);
void cmd_print_sizes(void);
void cmd_list_commands(void);
int cmd_list_commands_print(struct node *n);
void cmd_disp_inp(void);
void cmd_test_mouse(void);