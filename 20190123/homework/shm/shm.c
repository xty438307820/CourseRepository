#include "func.h"

int main(){
	key_t key=ftok("./file",1);
	int shmid=shmget(key,4096,0600|IPC_CREAT);
	if(-1==shmid){
		perror("shmget");
		return -1;
	}
	char *p=shmat(shmid,NULL,0);
	int fk=fork();
	if(0==fk){
		char buf[128]={0};
		strcpy(buf,p);
		printf("%s\n",buf);
	}
	else{
		strcpy(p,"How are you");
		wait(NULL);
	}
	int dm=shmdt(p);
	if(-1==dm){
		perror("shmdt");
		return -1;
	}
	return 0;
}
