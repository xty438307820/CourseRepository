#include "funcpp.h"

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

int transFile(int newfd,char* fileName){

    //传文件名
    char buf[1024]={0};
    int dataLen;
    dataLen=strlen(fileName);
    int ret=send_n(newfd,&dataLen,sizeof(int));
    if(-1==ret){
        return -1;
    }
    ret=send_n(newfd,fileName,dataLen);
    if(-1==ret){
        return -1;
    }

    //传文件大小
    int fd=open(fileName,O_RDONLY);
    struct stat fileStat;
    fstat(fd,&fileStat);
    long totalSize=fileStat.st_size;
    dataLen=sizeof(long);
    ret=send_n(newfd,&dataLen,sizeof(int));
    if(-1==ret){
        close(fd);
        return -1;
    }
    ret=send_n(newfd,&totalSize,dataLen);
    if(-1==ret){
        close(fd);
        return -1;
    }

    long currentSize=0,preSize=0;
    long distSize=totalSize/1000;//每0.1%更新进度条

    //传文件内容
    while(dataLen=read(fd,buf,sizeof(buf))){
        ret=send_n(newfd,&dataLen,sizeof(int));
        if(-1==ret){
            close(fd);
            return -1;
        }
        ret=send_n(newfd,buf,dataLen);
        if(-1==ret){
            close(fd);
            return -1;
        }
        currentSize+=dataLen;
        if(currentSize-preSize>=distSize){
            printf("%5.2f%s\r",(double)currentSize/totalSize*100,"%");
            fflush(stdout);
            preSize=currentSize;
        }
    }
    ret=send_n(newfd,&dataLen,sizeof(int));
    if(-1==ret){
        close(fd);
        return -1;
    }
    printf("100.0%s\n","%");
    close(fd);
    return 0;
}

int recvFile(int newfd,char* absPath){

    //接文件名
    char buf[1024]={0};
    int dataLen;
    int ret=recv_n(newfd,&dataLen,sizeof(int));
    if(-1==ret){
        return -1;
    }
    recv_n(newfd,buf+strlen(buf),dataLen);
    strcpy(buf,absPath);
    if(-1==ret){
        return -1;
    }

    //接文件大小
    int fd=open(buf,O_RDWR|O_CREAT,0666);
    long totalSize;
    ret=recv_n(newfd,&dataLen,sizeof(int));
    if(-1==ret){
        close(fd);
        return -1;
    }
    ret=recv_n(newfd,&totalSize,dataLen);
    if(-1==ret){
        close(fd);
        return -1;
    }

    long currentSize=0,preSize=0;
    long distSize=totalSize/1000;//每0.1%更新进度条

    //接文件内容
    while(1){
        ret=recv_n(newfd,&dataLen,sizeof(int));
        if(0==dataLen) break;
        if(-1==ret){
            close(fd);
            return -1;
        }
        ret=recv_n(newfd,buf,dataLen);
        if(-1==ret){
            close(fd);
            return -1;
        }
        write(fd,buf,dataLen);
        currentSize+=dataLen;
        if(currentSize-preSize>=distSize){
            printf("%5.2f%s\r",(double)currentSize/totalSize*100,"%");
            fflush(stdout);
            preSize=currentSize;
        }
    }
    printf("100.00%s\n","%");
    close(fd);
    return 0;
}
