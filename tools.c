#include "tools.h"

void anykey_continue(void)
{
	stdin->_IO_read_ptr = stdin->_IO_read_end;
	puts("按任意键继续...");
	getch();
	stdin->_IO_read_ptr = stdin->_IO_read_end;
}

void clear_stdin(void)
{
	stdin->_IO_read_ptr = stdin->_IO_read_end;
}

char* get_str(char* str,size_t len)
{
	clear_stdin();
	if(NULL == str)
	{
		puts("Pointer is NULL");
		return NULL;
	}
	fgets(str,len,stdin);
	size_t cnt = strlen(str);
	str[cnt-1] = '\0';
	clear_stdin();
	return str;
}

char get_sex(void)
{
	printf("m/w(男/女):");
	while(1)
	{
		char sex = getch();
		if('w' ==  sex || 'm' == sex)
		{
			printf("%s\n",'m' == sex ? "男" : "女");
			return sex;
		}
	}
}

char get_cmd(char start,char end)
{
	printf("请输入指令:");
	while(1)
	{
		char cmd = getch();
		printf("%c\n",cmd);
		if(cmd >= start && cmd <=end)
			return cmd;
		else
			printf("请重新输入指令:");
	}
}
/*
void input_pwd(char* pwd,size_t len)
{
	int i = 0;
	while(1)
	{
		stdin -> _IO_read_ptr = stdin -> _IO_read_end;
		pwd[i] = getch();
		fflush(stdin);
		if(i > len)
		{
			system("clear");
			memset(pwd,0,sizeof(char)*len);
			i=0;
			show_msg("请输入20位以下的密码！",1.0);
			anykey_continue();
			printf("请输入新密码：");
			continue;
		}
		if(pwd[i] == 127)
		{
			if(i > 0)
			{
				printf("\b \b");
				i--;
				pwd[i] = '\0';
				continue;
			}
			else
			{
				continue;
			}
		}
		else if(pwd[i] == 10)
		{
			pwd[i] = '\0';
			if(i < 6)
			{
				system("clear");
				memset(pwd,0,sizeof(char)*20);
				i=0;
				show_msg("请输入6位或6位以上的密码！",1.0);
				anykey_continue();
				printf("请重新输入密码：");
				continue;
			}
			stdin -> _IO_read_ptr = stdin -> _IO_read_end;
			break;
		}
		else
		{
			printf("*");
		}
		i++;
	}
	printf("\n");
}
*/

char* get_pwd(char* pwd,size_t len)
{
	if(NULL == pwd)	return NULL;
	int cnt = 0;
	while(cnt < len-1)
	{
		char val = getch();
		if(127 == val)
		{
			if(cnt > 0)
			{
				printf("\b \b");
				cnt--;
				continue;
			}
		}
		else if(10 == val)
		{
			break;
		}
		pwd[cnt++] = val;
		printf("*");
	}
	pwd[cnt] = '\0';
	return pwd;
}
