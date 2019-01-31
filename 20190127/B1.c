#include<func.h>

struct msgbuf{
	long mtype;
	char mtext[128];	
};

int main(){
	int msgid=msgget(1000,0600|IPC_CREAT);
	struct msgbuf mbuf;
	mbuf.mtype=1;
	char buf[128]={0};
	int ret;
	while(1){
		memset(mbuf.mtext,0,sizeof(mbuf.mtext));
		ret=msgrcv(msgid,&mbuf,sizeof(mbuf.mtext),0,0);
		strcpy(buf,mbuf.mtext);
		if(-1==ret) break;
		//自己发的消息
		if(buf[strlen(buf)-1]=='0'){
			buf[strlen(buf)-1]=0;
			printf("%50s\n",buf);
		}
		//对方发的消息
		else{
			buf[strlen(buf)-1]=0;
			printf("%s\n",buf);
		}
	}

	return 0;
}
