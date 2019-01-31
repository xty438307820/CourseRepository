#include<func.h>

void fun(int snum){
	printf("%d signal\n",snum);
	sleep(2);
	printf("%d after\n",snum);
}

int main(){
	signal(SIGINT,fun);
	signal(SIGQUIT,fun);
	while(1);
	return 0;
}
