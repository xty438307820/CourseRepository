#include <func.h>

int main(int argc,char* argv[])
{
    judgeArgc(argc,2);
    int sid=socket(AF_INET,SOCK_STREAM,0);
    printf("%d\n",sid);
    struct sockaddr_in sock;
    memset(&sock,0,sizeof(sock));
    sock.sin_addr.s_addr=inet_addr("192.168.135.130");
    sock.sin_port=htons(atoi(argv[1]));
    sock.sin_family=AF_INET;
    int ret=connect(sid,(struct sockaddr*)&sock,sizeof(struct sockaddr_in));
    if(-1==ret){
        perror("connect");
        return -1;
    }
    send(sid,"I am client",11,0);
    char buf[128]={0};
    recv(sid,buf,sizeof(buf),0);
    printf("%s\n",buf);
    close(sid);
    return 0;
}

