#include "func_pool.h"

int send_n(int newfd,void *data,int dataLen){
    int ret,total=0;
    char* snd=(char*)data;
    while(total<dataLen){
        ret=send(newfd,snd+total,dataLen-total,MSG_NOSIGNAL);//不被SIGPIPE中断
        if(-1==ret){
            return -1;
        }
        total+=ret;
    }
    return 0;
}

int recv_n(int newfd,void *data,int dataLen){
    int ret,total=0;
    char *rcv=(char*)data;
    while(total<dataLen){
        ret=recv(newfd,rcv+total,dataLen-total,0);
        if(0==ret){
            return -1;
        }
        total+=ret;
    }
    return 0;
}

int transFile(int newfd){

    //传文件名
    fileData t;
    strcpy(t.buf,"file");
    t.dataLen=strlen(t.buf);
    int ret=send_n(newfd,&t,t.dataLen+4);
    if(-1==ret){
        return -1;
    }

    //传文件大小
    int fd=open(t.buf,O_RDONLY);
    struct stat fileStat;
    fstat(fd,&fileStat);
    int totalSize=fileStat.st_size;
    t.dataLen=sizeof(int);
    ret=send_n(newfd,&t.dataLen,sizeof(int));
    if(-1==ret){
        close(fd);
        return -1;
    }
    ret=send_n(newfd,&totalSize,t.dataLen);
    if(-1==ret){
        close(fd);
        return -1;
    }

    while(t.dataLen=read(fd,t.buf,sizeof(t.buf))){
        ret=send_n(newfd,&t,t.dataLen+4);
        if(-1==ret){
            close(fd);
            return -1;
        }
    }
    ret=send_n(newfd,&t,t.dataLen+4);
    if(-1==ret){
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

