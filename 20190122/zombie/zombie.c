#include "func.h"

int main(){
	int fk=fork();
	if(fk==0){
		printf("I am child!\n");
		sleep(10);
		return 0;
	}
	else{
		printf("I am parent!\n");
		pid_t son=wait(NULL);
		printf("son %d has released\n",son);
		while(1);
		return 0;
	}
}
