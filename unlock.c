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

int unlock(Account* act)
{
	char filename[40] = {};
	sprintf(filename,"%ld.txt",act->user);
	int fd = open(filename,O_RDWR);
	if(0 > fd)
	{
		puts("帐号不存在");
		return -1;
	}
	Account buf = {};
	if(0 != read(fd,&buf,sizeof(Account)))
	{
		if(0 == strcmp(buf.id,act->id))
		{	
			buf.lock = 0;
			write(fd,&buf,sizeof(Account));
			close(fd);
			if(3 >= buf.lock)
			{
				puts("帐号已经解锁");
				return 1;
			}
			puts("此帐号未锁定");	
			return 0;
		}
		else
		{
			puts("密码错误");
			write(fd,&buf,sizeof(Account));
			act->lock = buf.lock;
			close(fd);
			return -1;
		}
	}
	else
	{
		puts("读取信息失败");
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
		//解锁进程接收611类型消息
		Msg msg = {};
		msgrcv(msgid,&msg,sizeof(Msg),611,MSG_NOERROR);
		printf("====用户解锁====\n");
		printf("user:%ld\n",msg.act.user);
		printf("id:%s\n",msg.act.id);
		
		msg.flag = unlock(&msg.act);
		msg.type = 116;
		msgsnd(msgid,&msg,sizeof(Msg),0);
		
	}
}
