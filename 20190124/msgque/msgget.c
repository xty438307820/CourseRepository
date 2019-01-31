#include <func.h>

struct msgbuf{
	long mtype;
	char mtest[20];
};

int main(){
	int msgid=msgget(1000,0666|IPC_CREAT);
	struct msgbuf buf;
	//int num=msgrcv(msgid,&buf,sizeof(buf.mtest),0,0);
	msgctl(msgid,IPC_RMID,NULL);
	return 0;
}
