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
    int ret=bind(sid,(struct sockaddr*)&sock,sizeof(struct sockaddr_in));
    if(-1==ret){
        perror("bind");
        return -1;
    }
    char buf[128]={0};
    struct sockaddr_in client;
    socklen_t addrlen=sizeof(struct sockaddr_in);
    recvfrom(sid,buf,sizeof(buf),0,(struct sockaddr*)&client,&addrlen);
    printf("%s %s %d\n",buf,inet_ntoa(client.sin_addr),ntohs(client.sin_port));
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
                    printf("Server close\n");
                    break;
                }
                sendto(sid,buf,strlen(buf)-1,0,(struct sockaddr*)&sock,sizeof(struct sockaddr_in));
            }
            if(FD_ISSET(sid,&set)){
                memset(buf,0,sizeof(buf));
                recvfrom(sid,buf,sizeof(buf),0,(struct sockaddr*)&sock,&addrlen);
                printf("%s\n",buf);
            }
        }
    }
    close(sid);
    return 0;
}
