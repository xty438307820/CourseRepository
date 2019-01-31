#include<func.h>

void fun1(int s){
	printf("this is ctrl+\\ signal\n");
	sleep(3);
}

void fun2(int s){
	printf("this is ctrl+c signal\n");
	sleep(3);
}

int main(){
	signal(SIGINT,fun2);
	signal(SIGQUIT,fun1);
	printf("process begin\n");
	while(1);
	return 0;
}
