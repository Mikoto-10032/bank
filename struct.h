#ifndef STRUCT_H
#define STRUCT_H

#define key (123456)
typedef struct Account
{
	long user;
	char name[20];
	char id[20];
	char pwd[20];
	double money;
	int lock;
}Account;

typedef struct Msg{
	long type;
	int flag;
	Account act;
}Msg;

#endif//STRUCT_H
