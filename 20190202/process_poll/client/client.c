#include <func.h>

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
    char buf[128]={0};
    recv(socketfd,buf,sizeof(buf),0);
    printf("%s\n",buf);
    close(socketfd);
    return 0;
}

