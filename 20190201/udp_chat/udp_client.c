#include <func.h>

int main(int argc,char* argv[])
{
    judgeArgc(argc,2);
    int sid=socket(AF_INET,SOCK_DGRAM,0);
    if(-1==sid){
        perror("socket");
        return -1;
    }
    struct sockaddr_in sock;
    memset(&sock,0,sizeof(sock));
    sock.sin_family=AF_INET;
    sock.sin_port=htons(atoi(argv[1]));
    sock.sin_addr.s_addr=inet_addr("192.168.135.130");
    sendto(sid,"hello",5,0,(struct sockaddr*)&sock,sizeof(struct sockaddr));
    int ret;
    char buf[128]={0};
    socklen_t addrlen;
    fd_set set;
    while(1){
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO,&set);
        FD_SET(sid,&set);
        ret=select(sid+1,&set,NULL,NULL,NULL);
        if(ret>0){
            if(FD_ISSET(STDIN_FILENO,&set)){
                memset(buf,0,sizeof(buf));
                ret=read(STDIN_FILENO,buf,sizeof(buf));
                if(0==ret){
                    printf("Client close\n");
                    break;
                }
                sendto(sid,buf,strlen(buf)-1,0,(struct sockaddr*)&sock,sizeof(struct sockaddr_in));
            }
            if(FD_ISSET(sid,&set)){
                memset(buf,0,sizeof(buf));
                ret=recvfrom(sid,buf,sizeof(buf),0,(struct sockaddr*)&sock,&addrlen);
                printf("%s\n",buf);
            }
        }
    }
    close(sid);
    return 0;
}

