#include <func.h>

int main()
{
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in sock;
    memset(&sock,0,sizeof(sock));
    sock.sin_family=AF_INET;
    sock.sin_port=htons(2000);
    sock.sin_addr.s_addr=inet_addr("192.168.135.130");
    bind(sfd,(struct sockaddr*)&sock,sizeof(sock));
    listen(sfd,10);

    int newfd=accept(sfd,NULL,NULL);
    printf("connect success\n");
    sleep(5);
    printf("start to send\n");
    int ret=send(newfd,"hello",5,0);
    printf("%d\n",ret);
    perror("send");
    ret=send(newfd,"world",5,MSG_NOSIGNAL);
    printf("%d\n",ret);
    perror("send");
    close(newfd);

    close(sfd);
    return 0;
}

