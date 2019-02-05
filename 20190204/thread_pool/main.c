#include "thread_factory.h"

void* threadFunc(void* p){

}

int main(int argc,char* argv[])
{
    judgeArgc(argc,3);//端口,线程数
    int threadNum=atoi(argv[2]);
    int socketfd=tcpInit(argv[1]);
    listen(socketfd,20);

    factory pf;
    memset(&pf,0,sizeof(pf));
    pf.pthid=(pthread_t*)calloc(threadNum,sizeof(pthread_t));
    pf.threadQue->Capacity=100;
    pthread_mutex_init(&pf.threadQue->queMutex,NULL);
    pthread_cond_init(&pf.cond,NULL);

    int i;
    for(i=0;i<threadNum;i++){

    }

    close(socketfd);
    return 0;
}

