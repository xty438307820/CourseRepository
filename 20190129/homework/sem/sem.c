#include<func.h>

#define N 10000000
typedef struct{
	sem_t sem;
    pthread_mutex_t mutex;
	int val;
}node;

void* add(void *p){
    int i;
    node* p1=(node*)p;
    for(i=0;i<N;i++){
        //sem_wait(&p1->sem);
        pthread_mutex_lock(&p1->mutex);
        p1->val++;
        pthread_mutex_unlock(&p1->mutex);
        //sem_post(&p1->sem);
    }
}

int main(){
	node thread;
	//sem_init(&thread.sem,0,1);
    pthread_mutex_init(&thread.mutex,NULL);
    struct timeval start,end;
    gettimeofday(&start,NULL);
    thread.val=0;
    pthread_t threadid1,threadid2;
    pthread_create(&threadid1,NULL,add,(void*)&thread);
    pthread_create(&threadid2,NULL,add,(void*)&thread);
    pthread_join(threadid1,NULL);
    pthread_join(threadid2,NULL);
    gettimeofday(&end,NULL);
    printf("%ld\n",(end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec);
    printf("%d\n",thread.val);
	return 0;
}
