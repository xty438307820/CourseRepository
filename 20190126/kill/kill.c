#include <func.h>

int main(){
	int id=getpid();
	printf("start\n");
	sleep(3);
	kill(id,SIGKILL);
	printf("you can't see me\n");
	return 0;
}
