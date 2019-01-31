#include "func.h"

int main(){
	int fk=fork();
	if(0==fk){
		printf("I am orphan,myppid=%d\n",getppid());
		while(1);
	}
	else{
		printf("I am parent\n");
	}
	return 0;
}
