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

int query(Account* act)
{
	char filename[40] = {};
	sprintf(filename,"%ld.txt",act->user);
	int fd = open(filename,O_RDONLY,0644);
	if(0 > fd)
	{
		puts("帐号不存在");
		return -1;
	}
	Account buf = {};
	read(fd,&buf,sizeof(Account));
	act->money = buf.money;
	*act = buf;
	//act = &buf;
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
		//查询进程接收211类型消息
		Msg msg = {};
		msgrcv(msgid,&msg,sizeof(Msg),211,MSG_NOERROR);
		
		printf("====用户查询====\n");
		printf("user:%ld\n",msg.act.user);
		int ret = query(&msg.act);
		if(0 != ret)
		{
			puts("用户不存在");
			msg.flag = -1;
		}
		else
		{
			printf("用户余额：%lf\n",msg.act.money);
		}
		msg.type = 112;
		msgsnd(msgid,&msg,sizeof(Msg),0);
		
	}
}
