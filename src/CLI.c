#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../inc/CLI.h"
#include "../inc/KSCbasicdrawN_fast.h"
#include "../inc/Serial.h"

//
//忽略字符串首部空格
char* ignore_space(char*str){
    if(str == NULL){
        return NULL;
    }
    while(*str == ' '){
        str++;
    }return str;
}
/*
 * @brief 切割字符串
 * 
 * @param str 原始字符串
 * @return char*  切割后的字符串(空格或'\0'或'\n'为截断点)
 */
char* cut_command(char* str){
	//return str;
	char* p=str;
	while((*p)!=' '&&(*p)!='\0'&&(*p)!='\n'){
		p++;
	}
	*p ='\0';
	return str;
}

/**
 * @brief 切割命令字符串
 * 
 * @param str 原始字符串
 * @param command 命令部分
 * @return char* 命令部分后的字符串,如果命令部分不存在或非首位,则返回NULL
 */
char* pip_command(char* str,const char* command){
    if(str == NULL || command == NULL){
        return NULL;
    }
    str = ignore_space(str);
    char* p = strstr(str,command);
    if(p == NULL){
        return NULL;
    }
    //if(*(p+strlen(command)) == '\0')printf("end of string\n");
    //命令部分在字符串开头,且命令部分后有一个空格或者为'\0'
    if(p == str&&((strncmp(p+strlen(command)," ",1) == 0)||*(p+strlen(command)) == '\0')){
        return ignore_space(p+strlen(command));
    }
    return NULL; 
}

cli_node cmd_system_table[] = {
    {NULL,NULL,NULL}
};

cli_node cmd_root_table[] = {
    {"system",NULL,cmd_system_table},
    {"draw",drawhelp,cmd_draw_table},
    //{"flash",kflashhelp,cmd_flash_table},
    //{"connect",kconnecthelp,cmd_connect_table},
    {NULL,NULL,NULL}
};

/**
 * @brief 运行CLI命令
 * 
 * @param str 命令字符串
 * @param table 命令表
 */
void run_cli(char* str,cli_node* table){
	if(str ==NULL){return;}
    if(table == NULL){
        table = cmd_root_table;
    }
    uint8_t i=0;
    char* fcommand = NULL;
    while(table[i].name != NULL){
        fcommand =pip_command(str,table[i].name);
        if(fcommand !=NULL){
            if(table[i].cmd_table != NULL){
                run_cli(fcommand,table[i].cmd_table);
            }
            else{
                table[i].func(fcommand);
            }
            return;
        }
        i++;
    }
	kprintf("not found command:%s",cut_command(str));
    return;
}
static int RUN_cli_callback = 0;
/**
 * @brief 设置运行CLI回调函数
 * 
 * @param callback 回调函数状态,0:不回调,1:回调到flash
 */
void set_run_cli_callback(uint8_t callback){
    RUN_cli_callback = callback;
    return;
}
/**
 * @brief 获取运行CLI回调函数状态
 * 
 * @return int 回调函数状态,0:不回调,1:回调到flash
 */
int get_run_cli_callback(void){
    return RUN_cli_callback;
}
/**
 * @brief 运行根命令
 * 
 * @param str 命令字符串
 */
void run_cli_root(char* str){
    switch(get_run_cli_callback()){
        case 0:
            run_cli(str,cmd_root_table);
            return;
		#if __USE_FLASH__ ==1
        case 1:
            kflashwrite_fast(str);
            break;
		#endif
        default:
            break;
    }
}
