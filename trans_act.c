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

int trans_act(Account* act,int flag)
{
	char filename[40] = {};
	sprintf(filename,"%ld.txt",act->user);
	int fd = open(filename,O_RDWR,0644);
	if(0 > fd)
	{
		puts("帐号不存在");
		return -1;
	}
	if(0 == flag)
	{
		printf("转入的目标账户：%ld\n",act->user);
		close(fd);
		return 0;
	}
	Account buf = {};
	read(fd,&buf,sizeof(Account));
	printf("原:%.2lf\n",buf.money);
	if(1 == flag)
	{
		buf.money += act->money;
	}
	else if(2 == flag)
	{
		buf.money -= act->money;
	}
	act->money = buf.money;
	printf("现:%.2lf\n",buf.money);
	fd = open(filename,O_RDWR|O_TRUNC,0644);
	write(fd,&buf,sizeof(Account));
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
		Msg msg = {};
		msgrcv(msgid,&msg,sizeof(Msg),311,MSG_NOERROR);		
		int ret = trans_act(&msg.act,msg.flag);
		if(0 != ret)
		{
			msg.flag = -1;
		}
		msg.type = 113;
		msgsnd(msgid,&msg,sizeof(Msg),0);
		
	}
}
