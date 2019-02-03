#include <func.h>

int main(int argc,char* argv[])
{
    judgeArgc(argc,2);
    int sid=socket(AF_INET,SOCK_STREAM,0);
    if(-1==sid){
        perror("socket");
        return -1;
    }
    struct sockaddr_in sock;
    memset(&sock,0,sizeof(struct sockaddr_in));
    sock.sin_family=AF_INET;
    sock.sin_port=htons(atoi(argv[1]));
    sock.sin_addr.s_addr=inet_addr("192.168.135.130");
    int ret=connect(sid,(struct sockaddr*)&sock,sizeof(struct sockaddr));
    if(-1==ret){
        perror("connect");
        return -1;
    }
    int epid=epoll_create(1);
    struct epoll_event ep,rec[2];
    ep.events=EPOLLIN;
    ep.data.fd=STDIN_FILENO;
    epoll_ctl(epid,EPOLL_CTL_ADD,STDIN_FILENO,&ep);
    ep.data.fd=sid;
    epoll_ctl(epid,EPOLL_CTL_ADD,sid,&ep);
    int i,waitnum;
    char buf[128]={0};
    while(1){
        waitnum=epoll_wait(epid,rec,2,-1);
        for(i=0;i<waitnum;i++){
            if(rec[i].data.fd==STDIN_FILENO){
                memset(buf,0,sizeof(buf));
                ret=read(STDIN_FILENO,buf,sizeof(buf));
                if(0==ret){
                    printf("Client close\n");
                    goto end;
                }
                send(sid,buf,strlen(buf)-1,0);
            }
            if(rec[i].data.fd==sid){
                memset(buf,0,sizeof(buf));
                ret=recv(sid,buf,sizeof(buf),0);
                if(0==ret){
                    printf("Connection close\n");
                    goto end;
                }
                printf("%s\n",buf);
            }
        }
    }

end:
    close(sid);
    return 0;
}

