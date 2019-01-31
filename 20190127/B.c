#include<func.h>

int source[3]={-1,-1,-1};//下标0、1为管道，下标2为消息队列

struct msgbuf{
	long mtype;
	char mtext[128];
};

void clear(){
	if(source[0]!=-1) close(source[0]);
	if(source[1]!=-1) close(source[1]);
	if(source[2]!=-1) msgctl(source[2],IPC_RMID,NULL);
	exit(0);
}

int main(){
	signal(SIGINT,clear);
	int fdr=open("file1",O_RDONLY);
	if(-1==fdr){
		perror("openr");
		return -1;
	}
	source[0]=fdr;
	int fdw=open("file2",O_WRONLY);
	if(-1==fdw){
		perror("openw");
		return -1;
	}
	source[1]=fdw;
	printf("Connection has been established\n");
	char pid[10]={0};

	//获取对方pid
	read(fdr,pid,sizeof(pid));
	int oid=atoi(pid);
	printf("%d\n",oid);

	//将自己pid写入写端
	memset(pid,0,sizeof(pid));
	sprintf(pid,"%d",getpid());
	write(fdw,pid,strlen(pid));

	//开辟消息队列
	int msgid=msgget(1000,0600|IPC_CREAT);
	struct msgbuf mbuf;
	mbuf.mtype=1;
	source[2]=msgid;

	//select实现两进程通信
	fd_set readSet;
	struct timeval t;
	t.tv_usec=0;
	char buf[128]={0};
	int ret;
	while(1){
		t.tv_sec=500;
		FD_ZERO(&readSet);
		FD_SET(STDIN_FILENO,&readSet);
		FD_SET(fdr,&readSet);
		if(select(fdr+1,&readSet,NULL,NULL,&t)>0){
			if(FD_ISSET(STDIN_FILENO,&readSet)){
				memset(buf,0,sizeof(buf));
				read(STDIN_FILENO,buf,sizeof(buf));
				write(fdw,buf,strlen(buf)-1);//不写换行
				buf[strlen(buf)-1]='0';//0为自己发的消息
				strcpy(mbuf.mtext,buf);
				msgsnd(msgid,&mbuf,strlen(buf),0);
			}
			if(FD_ISSET(fdr,&readSet)){
				memset(buf,0,sizeof(buf));
				ret=read(fdr,buf,sizeof(buf));
				if(0==ret){//对方关闭会话
					printf("Connection closed\n");
					clear();
					break;
				}
				buf[strlen(buf)]='1';//1为对方发的消息
				strcpy(mbuf.mtext,buf);
				msgsnd(msgid,&mbuf,strlen(buf),0);
				//printf("%s\n",buf);i
			}
		}
		else{
			printf("Connection time out\n");
			break;
		}
	}


	return 0;
}
