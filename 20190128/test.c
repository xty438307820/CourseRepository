#include<func.h>

void* fun(void* p){
	printf("I am child\n");
	while(1);
}

int main(){
	pid_t pid=getpid();
	pthread_t threadid=pthread_self();
	printf("pid=%d,threadid=%ld\n",pid,threadid);
	pthread_create(&threadid,NULL,fun,NULL);
	printf("%ld\n",threadid);
	while(1);
	return 0;
}
