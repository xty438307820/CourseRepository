#include <func.h>

int main()
{
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in sock;
    memset(&sock,0,sizeof(sock));
    sock.sin_family=AF_INET;
    sock.sin_port=htons(2000);
    sock.sin_addr.s_addr=inet_addr("192.168.135.130");
    connect(sfd,(struct sockaddr*)&sock,sizeof(sock));
    close(sfd);
    return 0;
}

