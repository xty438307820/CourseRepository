#include<func.h>

int main(){
	int shmid=shmget(1000,4096,0600|IPC_CREAT);
	char *p=(char*)shmat(shmid,NULL,0);
	char buf[128]={0};

	while(1){
		strcpy(buf,p);
		if(strlen(buf)>0){
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
			memset(p,0,strlen(buf));
		}
	}

	return 0;
}
