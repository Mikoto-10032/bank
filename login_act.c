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

int login(Account* act)
{
	char filename[40] = {};
	sprintf(filename,"%ld.txt",act->user);
	int fd = open(filename,O_RDWR,0644);
	if(0 > fd)
	{
		puts("帐号不存在");
		return -1;
	}
	Account buf = {};
	if(0 != read(fd,&buf,sizeof(Account)))
	{
		if(0 == strcmp(buf.pwd,act->pwd))
		{	
			if(3 <= buf.lock)
			{
				puts("帐号已经锁定");
				act->lock = buf.lock;
				fd = open(filename,O_RDWR|O_TRUNC,0644);
				write(fd,&buf,sizeof(Account));
				close(fd);
				return -1;
			}
			puts("登陆成功");
			buf.lock = 0;
			int fd = open(filename,O_RDWR|O_TRUNC,0644);
			write(fd,&buf,sizeof(Account));
			close(fd);
			return 1;
		}
		else
		{
			puts("密码错误");
			buf.lock++;
			act->lock = buf.lock;
			fd = open(filename,O_RDWR|O_TRUNC,0644);
			write(fd,&buf,sizeof(Account));
			close(fd);
			return -1;
		}
	}
	else
	{
		puts("读取信息失败");
		fd = open(filename,O_RDWR|O_TRUNC,0644);
		write(fd,&buf,sizeof(Account));
		close(fd);
		return -1;
	}
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
		//开户进程接收711类型消息
		Msg msg = {};
		msgrcv(msgid,&msg,sizeof(Msg),711,MSG_NOERROR);
		msg.flag = login(&msg.act);
		
		printf("====用户登陆====\n");
		printf("user:%ld\n",msg.act.user);
		printf("pwd:%s\n",msg.act.pwd);
		printf("lock:%d\n",msg.act.lock);
		
		msg.type = 117;
		msgsnd(msgid,&msg,sizeof(Msg),0);
		
	}
}
