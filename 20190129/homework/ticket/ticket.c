#include<func.h>

typedef struct{
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int ticketnum;
}node;

void* sale1(void* p){
	node* p1=(node*)p;
	while(1){
		pthread_mutex_lock(&p1->mutex);
		if(p1->ticketnum>0){
			printf("sale1 begin to sale,ticketnum=%d\n",p1->ticketnum);
			p1->ticketnum--;
			if(p1->ticketnum==0) pthread_cond_signal(&p1->cond);
			printf("sale1 finish sale,ticketnum=%d\n",p1->ticketnum);
			pthread_mutex_unlock(&p1->mutex);
			sleep(1);
		}
		else{
			pthread_mutex_unlock(&p1->mutex);
			break;
		}
	}
}

void* sale2(void* p){
	node *p1=(node*)p;
	while(1){
		pthread_mutex_lock(&p1->mutex);
		if(p1->ticketnum>0){
			printf("sale2 begin to sale,ticketnum=%d\n",p1->ticketnum);
			p1->ticketnum--;
			if(p1->ticketnum==0) pthread_cond_signal(&p1->cond);
			printf("sale2 finish sale,ticketnum=%d\n",p1->ticketnum);
			pthread_mutex_unlock(&p1->mutex);
			sleep(1);
		}
		else{
			pthread_mutex_unlock(&p1->mutex);
			break;
		}
	}
}

void* product(void *p){
	node *p1=(node*)p;
	pthread_mutex_lock(&p1->mutex);
	if(p1->ticketnum>0){
		pthread_cond_wait(&p1->cond,&p1->mutex);
		p1->ticketnum=10;
	}
	pthread_mutex_unlock(&p1->mutex);
}

int main(){
	node thread;
	pthread_mutex_init(&thread.mutex,NULL);
	pthread_cond_init(&thread.cond,NULL);
	thread.ticketnum=20;
	pthread_t thread1,thread2,thread3;
	pthread_create(&thread1,NULL,sale1,(void*)&thread);
	pthread_create(&thread2,NULL,sale2,(void*)&thread);
	pthread_create(&thread3,NULL,product,(void*)&thread);

	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	pthread_mutex_destroy(&thread.mutex);
	pthread_cond_destroy(&thread.cond);
	return 0;
}
