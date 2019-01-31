#include "func.h"

void fun(){
	printf("fun\n");
	exit(1);
}

int main(){
	int fk=fork();
	if(0==fk){
		printf("I am son\n");
		fun();
	}
	else{
		int status;
		wait(&status);
		if(WIFEXITED(status)){
			printf("son's return=%d\n",WEXITSTATUS(status));
		}
	}
	return 0;
}
