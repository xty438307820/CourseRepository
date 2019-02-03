#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<dirent.h>
#include<time.h>
#include<sys/time.h>
#include<pwd.h>
#include<grp.h>
#include<fcntl.h>
#include<sys/select.h>
#include<sys/time.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/msg.h>
#include<signal.h>
#include<sys/msg.h>
#include<pthread.h>
#include<semaphore.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/epoll.h>
#include<sys/uio.h>
#include<sys/mman.h>

#define judgeArgc(argc,num) {if(argc!=num){printf("arg err!\n");return -1;}}

typedef struct{
    pid_t pid;
    int fd;//子进程管道对端
    int busy;//0表示不忙,1表示忙
}procData;

typedef struct{
    int dataLen;
    char buf[1024];
}fileData;

int send_fd(int,int);
int recv_fd(int,int*);
int tcpInit(char*);
int transFile(int);
int send_n(int,void*,int);
int recv_n(int,void*,int);
