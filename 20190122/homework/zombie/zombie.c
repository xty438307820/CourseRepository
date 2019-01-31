#include "func.h"

int main(){
	int fk=fork();
	if(0==fk){
		printf("I am son\n");
	}
	else{
		printf("I am parent\n");
		while(1);
	}
	return 0;
}
