#ifndef __CLI_H__
#define __CLI_H__

#include "KSCconfig.h"
#include "Serial.h"

typedef int (*cmd_func)(char *argv);
typedef struct cmd_node{
    const char *name;
    cmd_func func;
    struct cmd_node *cmd_table;
}cli_node;


void run_cli(char* str,cli_node* table);
void set_run_cli_callback(uint8_t callback);
int get_run_cli_callback(void);
void run_cli_root(char* str);

#endif

