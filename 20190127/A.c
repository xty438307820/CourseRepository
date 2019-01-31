#include<func.h>

int source[3]={-1,-1,-1};//下标0、1为管道，小标2为共享内存

void clear(){
	if(source[0]!=-1) close(source[0]);
	if(source[1]!=-1) close(source[1]);
	if(source[2]!=-1) shmctl(source[2],IPC_RMID,NULL);
	exit(0);
}

int main(){
	signal(SIGINT,clear);
	int fdw=open("file1",O_WRONLY);
	if(-1==fdw){
		perror("openw");
		return -1;
	}
	source[0]=fdw;
	int fdr=open("file2",O_RDONLY);
	if(-1==fdr){
		perror("openr");
		return -1;
	}
	source[1]=fdw;
	printf("Connection has been established\n");
	char pid[10]={0};

	//将自己pid写入管道写端
	sprintf(pid,"%d",getpid());
	write(fdw,pid,strlen(pid));
	memset(pid,0,sizeof(pid));

	//获取对方pid
	read(fdr,pid,sizeof(pid));	
	int oid=atoi(pid);
	printf("%d\n",oid);

	//开辟共享内存
	int shmid=shmget(1000,4096,0600|IPC_CREAT);
	char *p=(char*)shmat(shmid,NULL,0);
	source[2]=shmid;

	//select实现两进程通信
	fd_set readSet;
	struct timeval t;
	t.tv_usec=0;
	char buf[128]={0};
	int ret;
	while(1){
		t.tv_sec=300;//五分钟无对话自动关闭连接
		FD_ZERO(&readSet);
		FD_SET(STDIN_FILENO,&readSet);
		FD_SET(fdr,&readSet);
		if(select(fdr+1,&readSet,NULL,NULL,&t)>0){
			if(FD_ISSET(STDIN_FILENO,&readSet)){
				memset(buf,0,sizeof(buf));
				read(STDIN_FILENO,buf,sizeof(buf));
				write(fdw,buf,strlen(buf)-1);//不写换行
				buf[strlen(buf)-1]='0';//0为自己发的消息
				strcpy(p,buf);
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
				strcpy(p,buf);
				//printf("%s\n",buf);
			}
		}
		else{
			printf("Connection time out\n");
			break;
		}
	}


	return 0;
}
