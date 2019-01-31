#include <func.h>

void print(){
	printf("world\n");
	char *c=(char*)malloc(20);
	strcpy(c,"helloworld");
	pthread_exit((void*)c);
}

void* fun(void* p){
	printf("I am chlid,p=%ld\n",(long)p);
	print();
	printf("after print\n");
}

int main(){
	pthread_t threadid;
	int ret=pthread_create(&threadid,NULL,fun,(void*)1);
	if(ret!=0){
		printf("error code=%d\n",ret);
		return -1;
	}
	char *c;
	pthread_join(threadid,(void**)&c);
	printf("%s\n",c);
	printf("I am parent\n");
	return 0;
}
