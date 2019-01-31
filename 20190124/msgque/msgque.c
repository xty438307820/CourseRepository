#include<func.h>

struct msgbuf{
	long mtype;
	char mtext[20];
};

int main(){
	int msgid=msgget(1000,0600|IPC_CREAT);
	struct msgbuf buf;
	buf.mtype=1;
	strcpy(buf.mtext,"hello");
	msgsnd(msgid,&buf,5,0);
	return 0;
}
