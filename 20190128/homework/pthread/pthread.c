#include<func.h>

void* print(void* p){
	sleep(2);
	printf("I am son,num=%ld\n",(long)p);
	pthread_exit((void*)2);
}

int main(){
	int ret;
	pthread_t threadid;
	ret=pthread_create(&threadid,NULL,print,(void*)1);
	if(ret!=0){
		printf("create error code=%d\n",ret);
		return -1;
	}
	long num;
	ret=pthread_join(threadid,(void**)&num);
	if(ret!=0){
		printf("join error code=%d\n",ret);
		return -1;
	}
	printf("I am parent,num=%ld\n",num);
	return 0;
}
