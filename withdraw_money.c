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

int withdraw_money(Account* act)
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
	read(fd,&buf,sizeof(Account));
	printf("原:%.2lf\n",buf.money);
	if(act->money > buf.money)
	{	
		return -1;
	}
	buf.money -= act->money;
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
		//查询进程接收411类型消息
		Msg msg = {};
		msgrcv(msgid,&msg,sizeof(Msg),411,MSG_NOERROR);
		
		printf("====用户取钱====\n");
		printf("user:%ld\n",msg.act.user);
		printf("取出金额:%.2lf",msg.act.money);
		
		int ret = withdraw_money(&msg.act);
		if(0 != ret)
		{
			puts("余额不足，取钱失败");
			msg.flag = -1;
		}
		msg.type = 114;
		msgsnd(msgid,&msg,sizeof(Msg),0);
		
	}
}
