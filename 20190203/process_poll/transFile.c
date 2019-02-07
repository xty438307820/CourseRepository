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

    while(t.dataLen=read(fd,t.buf,sizeof(t.buf))){
        send_n(newfd,&t,t.dataLen+4);
    }
    send_n(newfd,&t,t.dataLen+4);
    close(fd);
    return 0;
}

