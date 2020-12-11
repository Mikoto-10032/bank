#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "struct.h"
#include "tools.h"

int modify_pwd(Account* act)
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
	strcpy(buf.pwd,act->pwd);
	printf("新密码:%s",buf.pwd);
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
		msgrcv(msgid,&msg,sizeof(Msg),111,MSG_NOERROR);
		
		int ret = modify_pwd(&msg.act);
		if(0 != ret)
		{
			puts("改密失败");
			msg.flag = -1;
		}
		puts("改密成功");
		msg.type = 110;
		msgsnd(msgid,&msg,sizeof(Msg),0);
		
	}
}
