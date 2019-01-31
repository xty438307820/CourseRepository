#include<func.h>

int main(){
	pthread_t threadid;
	pthread_mutex_t lock;
	pthread_mutexattr_t at;
	pthread_mutexattr_settype(&at,PTHREAD_MUTEX_NORMAL);
	pthread_mutex_init(&lock,&at);
	int ret;
	ret=pthread_mutex_lock(&lock);
	printf("%d\n",ret);
	ret=pthread_mutex_destroy(&lock);
	printf("%d\n",ret);
	return 0;
}
