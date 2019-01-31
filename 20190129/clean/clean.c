#include<func.h>

void clean1(void* p){
	printf("clean1,char=%s\n",(char*)p);
}

void clean2(void *p){
	printf("clean2\n");
}

void clean3(void* p){
	printf("clean3\n");
}

void* fun(void* p){
	char ch[]="world";
	pthread_cleanup_push(clean1,ch);
	pthread_cleanup_push(clean2,NULL);
	pthread_cleanup_push(clean3,NULL);
	printf("I am child\n");
	pthread_exit(NULL);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
}

int main(){
	pthread_t threadid;
	pthread_create(&threadid,NULL,fun,NULL);
	printf("I am parent\n");
	pthread_join(threadid,NULL);
	return 0;
}
