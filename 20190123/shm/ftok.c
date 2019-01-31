#include "func.h"

int main(){
	key_t key=ftok("./file",1);
	int shmid=shmget(key,4096,0600|IPC_CREAT);
	
	
	return 0;
}
