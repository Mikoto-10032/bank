#ifndef TOOLS_H
#define TOOLS_H

#include <string.h>
#include <getch.h>
#include <stdbool.h>
#include <stdlib.h>

void anykey_continue(void);

void clear_stdin(void);

char* get_str(char* str,size_t len);

char get_sex(void);

char get_cmd(char start,char end);

char* get_pwd(char* pwd,size_t len);

void input_pwd(char* pwd,size_t len);
#endif//TOOLS_H
