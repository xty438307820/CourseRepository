#include <func.h>

void act(int isign){
	printf("%d is coming\n",isign);
	sleep(2);
}

int main(){
	signal(SIGINT,act);
	printf("process being\n");
	while(1);
	return 0;
}
