#include <func.h>

int main(int argc,char* argv[])
{
    judgeArgc(argc,2);
    int sid=socket(AF_INET,SOCK_STREAM,0);
    printf("%d\n",sid);
    if(-1==sid){
        perror("socket");
        return -1;
    }
    struct sockaddr_in sock;
    memset(&sock,0,sizeof(sock));
    sock.sin_addr.s_addr=inet_addr("192.168.135.130");
    sock.sin_family=AF_INET;
    sock.sin_port=htons(atoi(argv[1]));
    int ret=bind(sid,(struct sockaddr*)&sock,sizeof(struct sockaddr_in));
    if(ret==-1){
        perror("bind");
        return -1;
    }
    ret=listen(sid,10);
    if(-1==ret){
        perror("listen");
        return -1;
    }
    struct sockaddr_in client;
    socklen_t clen=sizeof(client);
    ret=accept(sid,(struct sockaddr*)&client,&clen);
    printf("%d\n",ret);
    printf("%s\n",inet_ntoa(client.sin_addr));
    printf("%d\n",ntohs(client.sin_port));
    if(-1==ret){
        perror("accept");
        return -1;
    }
    char buf[128]={0};
    recv(ret,buf,sizeof(buf),0);
    printf("%s\n",buf);
    send(ret,"I am Server",11,0);
    close(ret);
    close(sid);
    return 0;
}

