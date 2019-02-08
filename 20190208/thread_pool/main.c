#include "thread_factory.h"

void cleanFunc(void *p){
    pthread_mutex_unlock(&((pFactory)p)->threadQue->queMutex);
}

void* threadFunc(void* p){
    pthread_cleanup_push(cleanFunc,p);
    pFactory threadp=(pFactory)p;
    int ret;
    pNode pnode;
    while(1){
        pthread_mutex_lock(&threadp->threadQue->queMutex);
        if(0==threadp->threadQue->currentSize){
            pthread_cond_wait(&threadp->cond,&threadp->threadQue->queMutex);
        }
        ret=quePop(threadp->threadQue,&pnode);
        pthread_mutex_unlock(&threadp->threadQue->queMutex);
        if(0==ret){
            printf("thread %lu is busy\n",pthread_self());
            transFile(pnode->newfd);
            close(pnode->newfd);
            free(pnode);
            pnode=NULL;
            printf("thread %lu is not busy\n",pthread_self());
        }
    }
    pthread_cleanup_pop(1);
}

int sigpipe[2];
void sigFunc(int signum){
    write(sigpipe[1],&signum,4);
}

int main(int argc,char* argv[])
{
    judgeArgc(argc,3);//端口,线程数
    int threadNum=atoi(argv[2]);
    int socketfd=tcpInit(argv[1]);
    listen(socketfd,threadNum);

    factory f;
    memset(&f,0,sizeof(f));
    f.pthid=(pthread_t*)calloc(threadNum,sizeof(pthread_t));
    f.threadQue=(pQue)calloc(1,sizeof(Que));
    f.threadQue->Capacity=100;
    pthread_mutex_init(&f.threadQue->queMutex,NULL);
    pthread_cond_init(&f.cond,NULL);

    int i,j;
    for(i=0;i<threadNum;i++){
        pthread_create(f.pthid+i,NULL,threadFunc,&f);
        printf("thread %lu create success\n",f.pthid[i]);
    }

    //退出机制
    pipe(sigpipe);
    signal(SIGUSR1,sigFunc);


    //注册epoll
    int efd=epoll_create(1);
    struct epoll_event event;
    memset(&event,0,sizeof(event));
    event.events=EPOLLIN;
    event.data.fd=socketfd;
    epoll_ctl(efd,EPOLL_CTL_ADD,socketfd,&event);
    event.data.fd=sigpipe[0];
    epoll_ctl(efd,EPOLL_CTL_ADD,sigpipe[0],&event);

    int waitNum=0;
    struct epoll_event epArr[2];

    pNode pnode;
    int ret,newfd;
    struct sockaddr_in sock;
    memset(&sock,0,sizeof(sock));
    socklen_t addrlen=sizeof(sock);
    while(1){
        memset(epArr,0,sizeof(epArr));
        waitNum=epoll_wait(efd,epArr,2,-1);
        for(i=0;i<waitNum;i++){
            if(epArr[i].data.fd==socketfd){
                pnode=(pNode)calloc(1,sizeof(Node));
                newfd=accept(socketfd,(struct sockaddr*)&sock,&addrlen);
                printf("client %s %d connect\n",inet_ntoa(sock.sin_addr),ntohs(sock.sin_port));
                pnode->newfd=newfd;
                pthread_mutex_lock(&f.threadQue->queMutex);
                ret=queInsert(f.threadQue,pnode);
                pthread_mutex_unlock(&f.threadQue->queMutex);
                if(-1==ret){//队满,插入失败,直接关闭newfd
                    close(newfd);
                }
                pthread_cond_signal(&f.cond);
            }
            else if(epArr[i].data.fd==sigpipe[0]){
                close(socketfd);
                pNode closep;
                while(1){
                    pthread_mutex_lock(&f.threadQue->queMutex);
                    ret=quePop(f.threadQue,&closep);
                    pthread_mutex_unlock(&f.threadQue->queMutex);
                    if(-1==ret) break;
                    close(closep->newfd);
                }
                for(j=0;j<threadNum;j++){
                    pthread_cancel(f.pthid[j]);
                }
                for(j=0;j<threadNum;j++){
                    pthread_join(f.pthid[i],(void**)&ret);
                    if(-1==ret){
                        printf("%lu exit success\n",f.pthid[j]);
                    }
                    else{
                        printf("%lu exit something wrong\n",f.pthid[j]);
                    }
                }                
                exit(0);
            }
        }
    }

    close(socketfd);
    return 0;
}

