#include "func.h"

void fun(){
	printf("fun\n");
	abort();
}

int main(){
	pid_t fk=fork();
	if(fk==0){
		printf("I am child\n");
		fun();
	}
	else{
		int status=5;
		wait(&status);
		printf("I am parent\n");
		if(WIFEXITED(status)){
			printf("exit code=%d\n",WEXITSTATUS(status));
		}
		else{
			printf("exit crash\n");
		}
	}
	return 0;
}
