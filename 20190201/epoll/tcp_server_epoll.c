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
    sock.sin_family=AF_INET;
    sock.sin_port=htons(atoi(argv[1]));
    sock.sin_addr.s_addr=inet_addr("192.168.135.130");
    int ret=bind(sid,(struct sockaddr*)&sock,sizeof(struct sockaddr_in));
    if(-1==ret){
        perror("bind");
        return -1;
    }
    listen(sid,10);
    int epid=epoll_create(1);
    struct epoll_event ep,rec[3];
    ep.events=EPOLLIN;
    ep.data.fd=STDIN_FILENO;
    epoll_ctl(epid,EPOLL_CTL_ADD,STDIN_FILENO,&ep);
    ep.data.fd=sid;
    epoll_ctl(epid,EPOLL_CTL_ADD,sid,&ep);
    int i,waitnum,newid=-1;
    struct sockaddr_in client;
    socklen_t addrlen=sizeof(struct sockaddr_in);
    char buf[128]={0};
    while(1){
        waitnum=epoll_wait(epid,rec,3,-1);
        for(i=0;i<waitnum;i++){
            if(rec[i].data.fd==STDIN_FILENO){
                memset(buf,0,sizeof(buf));
                ret=read(STDIN_FILENO,buf,sizeof(buf));
                if(0==ret){
                    if(newid!=-1) close(newid);
                    printf("Server close\n");
                    goto end;
                }
                send(newid,buf,strlen(buf)-1,0);
            }
            if(rec[i].data.fd==newid){
                memset(buf,0,sizeof(buf));
                ret=recv(newid,buf,sizeof(buf),0);
                if(0==ret){
                    epoll_ctl(epid,EPOLL_CTL_DEL,newid,&ep);
                    close(newid);
                    continue;
                }
                printf("%s\n",buf);
            }
            if(rec[i].data.fd==sid){
                newid=accept(sid,(struct sockaddr*)&client,&addrlen);
                ep.data.fd=newid;
                epoll_ctl(epid,EPOLL_CTL_ADD,newid,&ep);
                printf("%s %d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
            }
        }
    }
end:
    close(sid);
    return 0;
}

