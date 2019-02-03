#include "func_pool.h"

//内核socket的send函数
int send_fd(int socketfd,int newfd){
    struct iovec iov;
    int buf=newfd;
    iov.iov_base=&buf;
    iov.iov_len=sizeof(int);

    int clen=CMSG_LEN(sizeof(int));
    struct cmsghdr cmsg;
    cmsg.cmsg_level=SOL_SOCKET;
    cmsg.cmsg_type=SCM_RIGHTS;
    cmsg.cmsg_len=clen;
    *(int*)CMSG_DATA(&cmsg)=newfd;
    if(-1==newfd){
        *(int*)CMSG_DATA(&cmsg)=0;
    }

    struct msghdr msg;
    memset(&msg,0,sizeof(msg));
    msg.msg_iov=&iov;
    msg.msg_iovlen=1;
    msg.msg_control=&cmsg;
    msg.msg_controllen=clen;
    
    int ret=sendmsg(socketfd,&msg,0);
    if(-1==ret){
        perror("sendmsg");
        return -1;
    }
    return 0;
}

//内核socket的receive函数
int recv_fd(int socketfd,int* newid){
    struct iovec iov;
    int exitflag;
    iov.iov_base=&exitflag;
    iov.iov_len=sizeof(int);

    int clen=CMSG_LEN(sizeof(int));
    struct cmsghdr cmsg;
    cmsg.cmsg_level=SOL_SOCKET;
    cmsg.cmsg_type=SCM_RIGHTS;
    cmsg.cmsg_len=clen;

    struct msghdr msg;
    memset(&msg,0,sizeof(msg));
    msg.msg_iov=&iov;
    msg.msg_iovlen=1;
    msg.msg_control=&cmsg;
    msg.msg_controllen=clen;

    int ret=recvmsg(socketfd,&msg,0);
    if(-1==ret){
        perror("recvmsg");
        return -1;
    }
    *newid=*(int*)CMSG_DATA((struct cmsghdr*)msg.msg_control);
    if(-1==exitflag){
        *newid=-1;
    }
    return 0;
}
