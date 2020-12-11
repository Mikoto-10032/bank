#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "struct.h"
#include "tools.h"

int judge(Account* act)
{
	char filename[40] = {};
	sprintf(filename,"%ld.txt",act->user);
	int fd = open(filename,O_RDONLY,0644);
	
	if(0 > fd)
	{
		puts("帐号不存在");
		return -2;	
	}
	
	Account buf = {};
	if(0 != read(fd,&buf,sizeof(Account)))
	{
		if(0 == strcmp(buf.name,act->name) && 0 == strcmp(buf.pwd,act->pwd))
		{
			close(fd);
			return 1;
		}
		else
		{
			puts("输入的信息有误");
			close(fd);
			return -1;
		}
	}
	else
	{
		puts("读取信息失败");
		close(fd);
		return -2;
	}
}

int del(Account act)
{
	char filename[40] = {};
	sprintf(filename,"%ld.txt",act.user);
	remove(filename);
	puts("销户成功");
	return 0;
}

int main()
{
	int msgid = msgget(key,0);
	if(0 > msgid)
	{
		perror("msgget");
		return -1;
	}
	
	for(;;)
	{	
		//开户进程接收811类型消息
		Msg msg = {};
		msgrcv(msgid,&msg,sizeof(Msg),811,MSG_NOERROR);
		printf("====用户销户====\n");
		printf("name:%s\n",msg.act.name);
		printf("user:%ld\n",msg.act.user);
		printf("pwd:%s\n",msg.act.pwd);
		
		if(0 == msg.flag)
		{
			msg.flag = judge(&msg.act);
		}
		else if(1 == msg.flag)
		{
			msg.flag = del(msg.act);
		}
		msg.type = 118;
		msgsnd(msgid,&msg,sizeof(Msg),0);
		
	}
}
