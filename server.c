#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "struct.h"
#include "tools.h"

static int msgid;
static pid_t prs_pid[9];

void sigint(int num)
{
	for(int i = 0; i < 9; i++)
	{
		kill(prs_pid[i],SIGKILL);
	}

	if(msgctl(msgid,IPC_RMID,NULL))
	{
		perror("msgctl");
	}
}

int main()
{
	signal(SIGINT,sigint);
	msgid = msgget(key,IPC_CREAT|IPC_EXCL|0644);
	if(0 > msgid)
	{
		perror("msgget");
		return -1;
	}
	
	prs_pid[0] = vfork();
	if(0 == prs_pid[0])
	{
		execl("open_act","open_act",NULL);
	}
	
	prs_pid[1] = vfork();
	if(0 == prs_pid[1])
	{
		execl("destroy","destroy",NULL);	
	}
	
	prs_pid[2] = vfork();
	if(0 == prs_pid[2])
	{
		execl("login_act","login_act",NULL);	
	}
	
	prs_pid[3] = vfork();
	if(0 == prs_pid[3])
	{
		execl("unlock","unlock",NULL);	
	}
	
	prs_pid[4] = vfork();
	if(0 == prs_pid[4])
	{
		execl("save_money","save_money",NULL);	
	}
	
	prs_pid[5] = vfork();
	if(0 == prs_pid[5])
	{
		execl("withdraw_money","withdraw_money",NULL);	
	}
	
	prs_pid[6] = vfork();
	if(0 == prs_pid[6])
	{
		execl("trans_act","trans_act",NULL);	
	}
	
	prs_pid[7] = vfork();
	if(0 == prs_pid[7])
	{
		execl("query","query",NULL);	
	}
	
	prs_pid[8] = vfork();
	if(0 == prs_pid[8])
	{
		execl("modify_pwd","modify_pwd",NULL);	
	}	
	
	while(-1 != wait(NULL));
}
