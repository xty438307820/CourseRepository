#include "func.h"

int main(){
	int fk=fork();
	if(0==fk){
		printf("son's pid=%d\n",getpid());
		while(1);	
	}
	else{
		printf("parent's pid=%d\n",getpid());
		while(1);
	}
	return 0;
}
