#include<func.h>

void* fun(void* p){
	//printf("I am son\n");
	while(1){
		printf("I am son\n");
		pthread_testcancel();
	}
	sleep(3);
}

int main(){
	pthread_t threadid;
	int ret=pthread_create(&threadid,NULL,fun,NULL);
	if(ret!=0){
		printf("create error code=%d\n",ret);
		return -1;
	}
	ret=pthread_cancel(threadid);
	if(ret!=0){
		printf("cancel error code=%d\n",ret);
		return -1;
	}
	long p;
	ret=pthread_join(threadid,(void**)&p);
	if(ret!=0){
		printf("join error code=%d\n",ret);
		return -1;
	}
	printf("%ld\n",p);
	return 0;
}
