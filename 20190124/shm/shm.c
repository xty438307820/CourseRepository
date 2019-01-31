#include "func.h"

int main(){
	int shmid=shmget(1000,4096,0600|IPC_CREAT);
	if(-1==shmid){
		perror("shmid");
		return -1;
	}
	while(1);
	int shmrm=shmctl(shmid,IPC_RMID,NULL);
	if(-1==shmrm){
		perror("shmctl");
		return -1;
	}
	return 0;
}
