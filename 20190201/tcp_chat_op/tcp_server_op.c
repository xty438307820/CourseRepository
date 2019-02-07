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
    memset(&sock,0,sizeof(sock));
    sock.sin_family=AF_INET;
    sock.sin_port=htons(atoi(argv[1]));
    sock.sin_addr.s_addr=inet_addr("192.168.135.130");
    int reuse=1;
    int ret=setsockopt(sid,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    if(-1==ret){
        perror("setsockopt");
        return -1;
    }
    ret=bind(sid,(struct sockaddr*)&sock,sizeof(struct sockaddr_in));
    if(-1==ret){
        perror("bind");
        return -1;
    }
    listen(sid,10);
    struct sockaddr_in client;
    socklen_t addrlen=sizeof(struct sockaddr_in);
    int newid;
    int recvlow=10;
    fd_set set,recordset;
    FD_ZERO(&recordset);
    FD_SET(STDIN_FILENO,&recordset);
    FD_SET(sid,&recordset);
    char buf[128]={0};
    while(1){
        memcpy(&set,&recordset,sizeof(fd_set));
        ret=select(15,&set,NULL,NULL,NULL);
        if(ret>0){
            if(FD_ISSET(STDIN_FILENO,&set)){
                memset(buf,0,sizeof(buf));
                ret=read(STDIN_FILENO,buf,sizeof(buf));
                if(0==ret){
                    printf("Server close\n");
                    break;
                }
                send(newid,buf,strlen(buf)-1,0);
            }
            if(FD_ISSET(newid,&set)){
                memset(buf,0,sizeof(buf));
                ret=recv(newid,buf,sizeof(buf),0);
                if(0==ret){
                    FD_CLR(newid,&recordset);
                    close(newid);
                    continue;
                }
                printf("%s\n",buf);
            }
            if(FD_ISSET(sid,&set)){
                newid=accept(sid,(struct sockaddr*)&client,&addrlen);
                setsockopt(newid,SOL_SOCKET,SO_RCVLOWAT,&recvlow,sizeof(int));
                setsockopt(newid,SOL_SOCKET,SO_SNDLOWAT,&recvlow,sizeof(int));
                FD_SET(newid,&recordset);
                printf("%s %d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
            }
        }
    }
    close(newid);
    close(sid);
    return 0;
}

