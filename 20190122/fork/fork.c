#include "func.h"

int main(){
	printf("333\n");
	int fk=fork();
	if(fk==0){
		printf("%d:child,%d\n",getpid(),getppid());
	}
	else{
		sleep(5);
		printf("%d\n",fk);
		printf("%d:parent\n",getpid());
	}
	printf("%d:exit()\n",getpid());
	return 0;
}
