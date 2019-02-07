#include <func.h>

int main()
{
    int fds[2];
    socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
    if(!fork()){
        close(fds[0]);
        int fd,send=600;
        printf("init\n");
        write(fds[1],&send,sizeof(int));
        sleep(5);
        read(fds[1],&fd,sizeof(int));
        printf("child recv=%d\n",fd);
        read(fds[1],&fd,sizeof(int));
        printf("child recv=%d\n",fd);
    }
    else{
        close(fds[1]);
        int fd,send=100;
        write(fds[0],&send,sizeof(int));
        send=200;
        write(fds[0],&send,sizeof(int));
        sleep(5);
        read(fds[0],&fd,sizeof(int));
        printf("parent recv=%d\n",fd);
        wait(NULL);
    }
    return 0;
}

