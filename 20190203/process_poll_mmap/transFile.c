#include "func_pool.h"

int send_n(int newfd,void *data,int dataLen){
    int ret,total=0;
    char* snd=(char*)data;
    while(total<dataLen){
        ret=send(newfd,snd+total,dataLen-total,0);
        total+=ret;
    }
    return 0;
}

int recv_n(int newfd,void *data,int dataLen){
    int ret,total=0;
    char *rcv=(char*)data;
    while(total<dataLen){
        ret=recv(newfd,rcv+total,dataLen-total,0);
        total+=ret;
    }
    return 0;
}

int transFile(int newfd){

    //传文件名
    fileData t;
    strcpy(t.buf,"file");
    t.dataLen=strlen(t.buf);
    int fd=open(t.buf,O_RDONLY);
    send_n(newfd,&t,t.dataLen+4);

    //传文件大小
    struct stat fileStat;
    fstat(fd,&fileStat);
    int totalSize=fileStat.st_size;
    t.dataLen=sizeof(int);
    send_n(newfd,&t.dataLen,sizeof(int));
    send_n(newfd,&totalSize,t.dataLen);

    char *p=(char*)mmap(NULL,totalSize,PROT_READ,MAP_SHARED,fd,0);
    int currentSize=0;

    t.dataLen=1024;
    while(currentSize<totalSize){
        if(totalSize-currentSize<t.dataLen) t.dataLen=totalSize-currentSize;
        send_n(newfd,&t.dataLen,sizeof(int));
        send_n(newfd,p+currentSize,t.dataLen);
        currentSize+=t.dataLen;
    }
    t.dataLen=0;
    send_n(newfd,&t,t.dataLen+4);
    munmap(p,totalSize);
    close(fd);
    return 0;
}

