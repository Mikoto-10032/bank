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

long user = 1001;

int create_user(Account* act)
{
	act->user = user;
	act->money = 0;
	act->lock = 0;
	char filename[40] = {};
	sprintf(filename,"%ld.txt",act->user);
	int fd = open(filename,O_RDWR|O_CREAT|O_EXCL,0644);
	if(0 > fd)
	{
		perror("open");
		return -1;
	}
	write(fd,act,sizeof(Account));
	user++;
	puts("保存账户成功");
	close(fd);
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
		//开户进程接收911类型消息
		Msg msg = {};
		msgrcv(msgid,&msg,sizeof(Msg),911,MSG_NOERROR);
		
		printf("====用户开户====\n");
		printf("name:%s\n",msg.act.name);
		printf("id:%s\n",msg.act.id);
		printf("pwd:%s\n",msg.act.pwd);
		
		int result = create_user(&msg.act);
		if(-1 == result)
		{
			msg.act.user = -1;
		}
		msg.type = 119;
		msgsnd(msgid,&msg,sizeof(Msg),0);
		
	}
}
