FLAG=-Werror -DDEBUG
STD=-std=gnu99
CC=gcc


all:
	$(CC) $(STD) $(FLAG) client.c tools.c -o client
	$(CC) $(STD) $(FLAG) server.c tools.c -o server
	$(CC) $(STD) $(FLAG) open_act.c tools.c -o open_act
	$(CC) $(STD) $(FLAG) destroy.c tools.c -o destroy
	$(CC) $(STD) $(FLAG) login_act.c tools.c -o login_act
	$(CC) $(STD) $(FLAG) unlock.c tools.c -o unlock
	$(CC) $(STD) $(FLAG) save_money.c tools.c -o save_money
	$(CC) $(STD) $(FLAG) withdraw_money.c tools.c -o withdraw_money
	$(CC) $(STD) $(FLAG) trans_act.c tools.c -o trans_act
	$(CC) $(STD) $(FLAG) query.c tools.c -o query
	$(CC) $(STD) $(FLAG) modify_pwd.c tools.c -o modify_pwd
clean:
	rm client server open_act destroy login_act unlock save_money withdraw_money trans_act query modify_pwd
