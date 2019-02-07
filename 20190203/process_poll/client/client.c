#include "func_pool.h"

int main(int argc,char* argv[])
{
    judgeArgc(argc,2);
    int socketfd=socket(AF_INET,SOCK_STREAM,0);
    if(-1==socketfd){
        perror("socket");
        return -1;
    }
    struct sockaddr_in sock;
    memset(&sock,0,sizeof(sock));
    sock.sin_family=AF_INET;
    sock.sin_port=htons(atoi(argv[1]));
    sock.sin_addr.s_addr=inet_addr("192.168.135.130");
    int ret=connect(socketfd,(struct sockaddr*)&sock,sizeof(struct sockaddr_in));
    if(-1==ret){
        perror("connect");
        return -1;
    }
    int dataLen;
    char buf[1024]={0};

    recv_n(socketfd,&dataLen,sizeof(int));
    recv_n(socketfd,buf,dataLen);

    int totalSize,currentSize=0;
    recv_n(socketfd,&dataLen,sizeof(int));
    recv_n(socketfd,&totalSize,dataLen);
 
    printf("%d\n",totalSize);
    int fd=open(buf,O_RDWR|O_CREAT,0666);

    time_t start=time(NULL);
    time_t end;
    while(1){
        recv_n(socketfd,&dataLen,sizeof(int));
        if(dataLen>0){
            recv_n(socketfd,buf,dataLen);
            currentSize+=dataLen;
            write(fd,buf,dataLen);
            end=time(NULL);
            if(end-start>=1){
                printf("%5.2f%s\r",(double)currentSize/totalSize*100,"%");
                fflush(stdout);
                start=end;
            }
        }
        else{
            close(fd);
            break;
        }
    }

    printf("100.00%s\n","%");
    printf("download success\n");
    close(socketfd);
    return 0;
}

