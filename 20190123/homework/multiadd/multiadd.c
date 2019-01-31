#include "func.h"

#define N 10000000

int main(){
	key_t key=ftok("./file",1);
	int shmid=shmget(key,4096,0600|IPC_CREAT);
	if(-1==shmid){
		perror("shmget");
		return -1;
	}
	int *p=(int*)shmat(shmid,NULL,0);
	if((int*)-1==p){
		perror("shmat");
		return -1;
	}
	p[0]=0;
	int fk=fork();
	if(0==fk){
		int i;
		for(i=0;i<N;i++) p[0]=p[0]+1;
	}
	else{
		int i;
		for(i=0;i<N;i++) p[0]=p[0]+1;
		wait(NULL);
		printf("%d\n",p[0]);
	}
	int dm=shmdt(p);
	if(-1==dm){
		perror("shmdt");
		return -1;
	}
	return 0;
}
