#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include "struct.h"
#include "tools.h"

int msgid;
int init(void);			//初始化获取消息队列
Account online_user = {};		//登陆成功的在线用户

void menu(void);		//主界面
void open_act(void);		//开户
void destroy_act(void);		//销户
void login(void);		//登陆
void unlock(void);		//解锁

void user_menu(void);			//用户界面
void save_money(void);			//存钱
void withdraw_money(void);	//取钱
void trans_act(void);			//转帐
void query(void);				//查询
void modify_pwd(void);			//修改密码


int main()
{
	init();
	menu();
}

//初始化获取消息队列
int init(void)
{
	//debug("%s\n",__func__);
	//获取消息队列
	msgid = msgget(key,0);
	if(0 > msgid)
	{
		perror("msgget");
		return -1;
	}
	return 0;
}

//开户
void open_act(void)
{
	Account act = {};
	//发送信息类型定为 911，接受类型 119
	Msg msg = {};
	msg.type = 911;
	printf("请输入姓名:");
	get_str(act.name,20);
	printf("请输入身份证:");
	get_str(act.id,20);
	printf("请输入密码:");
	get_str(act.pwd,20);
	
	msg.act = act;
	//向服务器端发送注册信息
	msgsnd(msgid,&msg,sizeof(Msg),0);
	
	//接收服务器端进程返回的119类型消息
	msgrcv(msgid,&msg,sizeof(Msg),119,MSG_NOERROR);
	if(-1 == msg.act.user)
	{
		puts("尊敬的客户，由于系统维护，开户失败了");
		anykey_continue();
		return;
	}
	printf("恭喜%s，开户成功\n",msg.act.name);
	printf("您的帐号为%ld\n",msg.act.user);
	anykey_continue();
	return;
}

//销户
void destroy_act(void)
{
	Account act = {};
	//发送信息类型定为 811，接受类型 118
	Msg msg = {};
	msg.type = 811;
	msg.flag = 0;
	printf("请输入姓名:");
	get_str(act.name,20);
	printf("请输入帐号:");
	scanf("%ld",&act.user);
	getchar();
	printf("请输入密码:");
	get_str(act.pwd,20);
	msg.act = act;
	//向服务器端发送注册信息
	msgsnd(msgid,&msg,sizeof(Msg),0);
	//接收开发进程返回的118类型消息
	msgrcv(msgid,&msg,sizeof(Msg),118,MSG_NOERROR);
	if(1 == msg.flag)
	{	
		for(;;)
		{
			printf("是否确认销户（Y/N）：");
			char cmd = getchar();
			if('Y' == cmd || 'y' == cmd)
			{
				msg.type = 811;
				msgsnd(msgid,&msg,sizeof(Msg),0);
				msgrcv(msgid,&msg,sizeof(Msg),118,MSG_NOERROR);
				if(0 == msg.flag)
				{
					puts("销户成功！");
				}
				else
				{
					puts("销户失败！");
				}
				break;
			}
			else if('N' == cmd || 'n' == cmd)
			{
				puts("已经取消销户！");
				break;
			}
		}
	}
	else if(-2 == msg.flag)
	{
		puts("此帐号不存在，销户失败！");
	}
	else if(-1 == msg.flag)
	{
		puts("输入的密码或者姓名错误，销户失败！");
	}
	
	anykey_continue();
	return;
}

//登陆
void login(void)
{
	Account act = {};
	//发送信息类型定为 711，接受类型 117
	Msg msg = {};
	msg.type = 711;
	msg.flag = 0;
	printf("请输入帐号:");
	scanf("%ld",&act.user);
	//getchar();
	printf("请输入密码:");
	get_str(act.pwd,20);
	msg.act = act;
	//向服务器端发送注册信息
	msgsnd(msgid,&msg,sizeof(Msg),0);
	//接收开发进程返回的117类型消息
	msgrcv(msgid,&msg,sizeof(Msg),117,MSG_NOERROR);
	if(3 <= msg.act.lock)
	{
		puts("密码输错3次，此账户已锁定，请解锁");
		anykey_continue();
		return;
	}
	
	if(1 == msg.flag)
	{
		puts("登陆成功");
		online_user = msg.act;
		anykey_continue();
		user_menu();
	}
	else
	{
		puts("登陆失败，帐号或者密码错误！");
	}
	anykey_continue();
	return;
}

//解锁
void unlock(void)
{
	Account act = {};
	//发送信息类型定为 611，接受类型 116
	Msg msg = {};
	msg.type = 611;
	msg.flag = 0;
	printf("请输入帐号:");
	scanf("%ld",&act.user);
	//getchar();
	printf("请输入身份证:");
	get_str(act.id,20);
	msg.act = act;
	msgsnd(msgid,&msg,sizeof(Msg),0);
	msgrcv(msgid,&msg,sizeof(Msg),116,MSG_NOERROR);
		
	if(1 == msg.flag)
		puts("解锁成功");
	else if(0 == msg.flag)
		puts("此帐号并未锁定");
	else
		puts("解锁失败，输入的帐号或者身份证错误！");
	anykey_continue();
	return;
}

//存钱
void save_money(void)
{
	Msg msg = {};
	//发送信息类型定为 511，接受类型 115
	msg.type = 511;
	msg.flag = 0;
	msg.act = online_user;
	
	printf("请输入要存入的金额:");
	scanf("%lf",&msg.act.money);

	msgsnd(msgid,&msg,sizeof(Msg),0);
	msgrcv(msgid,&msg,sizeof(Msg),115,MSG_NOERROR);
	if(-1 != msg.flag)
	{
		online_user.money = msg.act.money;
		printf("======存钱成功======\n");
		printf("用户余额：%.2lf\n",msg.act.money);
	}
	else
		printf("系统正在升级，存钱失败\n");
	anykey_continue();
	return;
}

//取钱
void withdraw_money(void)
{
	Msg msg = {};
	//发送信息类型定为 411，接受类型 114
	msg.type = 411;
	msg.flag = 0;
	msg.act = online_user;
	
	printf("请输入要取出的金额:");
	scanf("%lf",&msg.act.money);

	msgsnd(msgid,&msg,sizeof(Msg),0);
	msgrcv(msgid,&msg,sizeof(Msg),114,MSG_NOERROR);
	if(-1 != msg.flag)
	{
		online_user.money = msg.act.money;
		printf("======取钱成功======\n");
		printf("用户余额：%.2lf\n",msg.act.money);
	}
	else
		printf("余额不足，取钱失败\n");
	anykey_continue();
	return;
}

//转帐
void trans_act(void)
{
	Msg msg = {};
	//发送信息类型定为 311，接受类型 113
	msg.type = 311;
	msg.flag = 0;
	msg.act.money = 0;
	printf("请输入要转帐的账户:");
	scanf("%ld",&msg.act.user);
	//发送信息，查询目标账户是否存在
	msgsnd(msgid,&msg,sizeof(Msg),0);
	msgrcv(msgid,&msg,sizeof(Msg),113,MSG_NOERROR);
	if(0 != msg.flag)
	{
		puts("您要转入的账户不存在，转帐失败");
		anykey_continue();
		return;
	}
	printf("请输入要转帐的金额:");
	scanf("%lf",&msg.act.money);
	if(msg.act.money > online_user.money)
	{
		puts("余额不足，转帐失败");
		anykey_continue();
		return;
	}
	//再次发送信息，给目标账户加钱
	msg.flag = 1;
	msg.type = 311;
	msgsnd(msgid,&msg,sizeof(Msg),0);
	msgrcv(msgid,&msg,sizeof(Msg),113,MSG_NOERROR);
	//再次发送信息，给账户减钱
	msg.type = 311;
	msg.flag = 2;
	msg.act.user = online_user.user;
	msgsnd(msgid,&msg,sizeof(Msg),0);
	msgrcv(msgid,&msg,sizeof(Msg),113,MSG_NOERROR);
	online_user.money = msg.act.money;
	
	puts("转帐成功");
	printf("用户余额：%.2lf\n",msg.act.money);
	anykey_continue();
	return;
}

//查询
void query(void)
{
	Msg msg = {};
	//发送信息类型定为 211，接受类型 112
	msg.type = 211;
	msg.flag = 0;
	msg.act = online_user;
	msgsnd(msgid,&msg,sizeof(Msg),0);
	msgrcv(msgid,&msg,sizeof(Msg),112,MSG_NOERROR);
	if(-1 != msg.flag)
		printf("用户余额：%.2lf\n",msg.act.money);
	else
		printf("查询失败\n");
	anykey_continue();
	return;
}

//修改密码
void modify_pwd(void)
{
	Msg msg = {};
	//发送信息类型定为 111，接受类型 110
	msg.type = 111;
	msg.flag = 0;
	msg.act.user = online_user.user;
	
	printf("请输入新密码:");
	get_str(msg.act.pwd,20);
	
	msgsnd(msgid,&msg,sizeof(Msg),0);
	msgrcv(msgid,&msg,sizeof(Msg),110,MSG_NOERROR);
	if(0 == msg.flag)
		printf("改密成功\n");
	else
		printf("改密失败\n");
	anykey_continue();
	return;
}

//用户界面
void user_menu(void)
{
	//debug("%s\n",__func__);
	while(1)
	{
		system("clear");
		printf("---  银行  ---\n");
		printf("1、  存钱\n");
		printf("2、  取钱\n");
		printf("3、  转帐\n");
		printf("4、  查询\n");
		printf("5、  改密\n");
		printf("0、  退出\n");
		switch(get_cmd('0','5'))
		{
			case '1':save_money(); break;
			case '2':withdraw_money(); break;
			case '3':trans_act(); break;
			case '4':query(); break;
			case '5':modify_pwd(); break;
			case '0':return;
		}
	}
}

//主界面
void menu(void)
{
	//debug("%s\n",__func__);
	while(1)
	{
		system("clear");
		printf("---  银行  ---\n");
		printf("1、  开户\n");
		printf("2、  销户\n");
		printf("3、  登陆\n");
		printf("4、  解锁\n");
		printf("0、  退出\n");
		switch(get_cmd('0','4'))
		{
			case '1':open_act(); break;
			case '2':destroy_act(); break;
			case '3':login(); break;
			case '4':unlock(); break;
			case '0':return;
		}
	}
}


