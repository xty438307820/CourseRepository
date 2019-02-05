#include "func_pool.h"

void createChild(procData *childProc,int childnum){
    int i,fk;
    int fds[2];
    for(i=0;i<childnum;i++){
        socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
        fk=fork();
        if(!fk){
            int newfd;
            close(fds[1]);
            while(1){
                recv_fd(fds[0],&newfd);
                if(-1==newfd){
                    close(fds[0]);
                    exit(0);
                }
                int ret=transFile(newfd);
                if(-1==ret){
                    printf("Client close\n");
                }
                close(newfd);
                write(fds[0],"finish",6);
            }
        }
        childProc[i].pid=fk;;
        close(fds[0]);
        childProc[i].fd=fds[1];
    }
}

int sigpipe[2];
void sigfunc(int signum){
    write(sigpipe[1],&signum,sizeof(int));
} 

int main(int argc,char* argv[])
{
    judgeArgc(argc,3);

    //tcp监听
    int socketfd=tcpInit(argv[1]);
    if(-1==socketfd){
        perror("tcpInit");
        return -1;
    }

    //创建子进程
    int i,j;
    int childnum=atoi(argv[2]);
    procData *childProc=(procData*)calloc(childnum,sizeof(procData));
    createChild(childProc,childnum);
    for(i=0;i<childnum;i++){
        printf("%d %d\n",childProc[i].pid,childProc[i].fd);
    }
    listen(socketfd,10);

    //信号处理
    pipe(sigpipe);
    signal(SIGUSR1,sigfunc);

    //创建epoll
    int epfd=epoll_create(1);
    struct epoll_event event;
    memset(&event,0,sizeof(event));
    event.events=EPOLLIN;
    event.data.fd=socketfd;
    int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,socketfd,&event);
    if(-1==ret){
        perror("epoll_ctl");
        return -1;
    }
    event.data.fd=sigpipe[0];
    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,sigpipe[0],&event);

    //epoll监听
    int waitnum,newfd;
    struct epoll_event *clientEp=(struct epoll_event*)calloc(childnum+1,sizeof(struct epoll_event));
    struct sockaddr_in client;
    socklen_t addrlen=sizeof(struct sockaddr_in);
    char buf[128]={0};
    while(1){
        memset(clientEp,0,(childnum+1)*sizeof(struct epoll_event));
        waitnum=epoll_wait(epfd,clientEp,childnum+1,-1);
        for(i=0;i<waitnum;i++){
            if(clientEp[i].data.fd==socketfd){
               newfd=accept(socketfd,(struct sockaddr*)&client,&addrlen);
               printf("client ip=%s,port=%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
               for(j=0;j<childnum;j++){
                   if(childProc[j].busy==0){
                       send_fd(childProc[j].fd,newfd);
                       close(newfd);
                       event.data.fd=childProc[j].fd;
                       epoll_ctl(epfd,EPOLL_CTL_ADD,childProc[j].fd,&event);
                       childProc[j].busy=1;
                       printf("%d is busy\n",childProc[j].pid);
                       break;
                   }
               }
            }
            for(j=0;j<childnum;j++){
                if(childProc[j].fd==clientEp[i].data.fd){
                    memset(buf,0,sizeof(buf));
                    read(clientEp[i].data.fd,buf,sizeof(buf));
                    event.data.fd=childProc[j].fd;
                    epoll_ctl(epfd,EPOLL_CTL_DEL,childProc[j].fd,&event);
                    printf("%d is not busy\n",childProc[j].pid);
                    childProc[j].busy=0;
                    break;
                }
            }
            if(clientEp[i].data.fd==sigpipe[0]){
                read(sigpipe[0],buf,sizeof(buf));
                event.data.fd=socketfd;
                epoll_ctl(epfd,EPOLL_CTL_DEL,socketfd,&event);
                close(socketfd);
                for(j=0;j<childnum;j++){
                    send_fd(childProc[j].fd,-1);
                }
                for(j=0;j<childnum;j++){
                    wait(NULL);
                    printf("exit success\n");
                }
                for(j=0;j<childnum;j++){
                    close(childProc[j].fd);
                }
                exit(0);
            }
        }
    }

    close(socketfd);
    return 0;
}

