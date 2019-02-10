#include "funcpp.h"

void splitChar(char*,char*,char*);//分割char*,将中间空格分开,后两个参数为传出参数

int main(int argc,char* argv[])
{
    judgeArgc(argc,3);
    
    //客户端连接
    int socketfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in sock;
    memset(&sock,0,sizeof(sockaddr_in));
    sock.sin_family=AF_INET;
    sock.sin_port=htons(atoi(argv[2]));
    sock.sin_addr.s_addr=inet_addr(argv[1]);
    int ret=connect(socketfd,(struct sockaddr*)&sock,sizeof(sockaddr_in));
    if(-1==ret){
        perror("connect");
        return -1;
    }
    cout<<"connect success"<<endl;

    char buf[128]={0};
    char operate[32]={0};
    char operand[96]={0};
    int dataLen,fd;
    long fileSize;

    while(1){
        memset(buf,0,sizeof(buf));
        memset(operate,0,sizeof(operate));
        memset(operand,0,sizeof(operand));
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf)-1]=0;
        dataLen=strlen(buf);
        ret=send_n(socketfd,&dataLen,sizeof(int));
        if(-1==ret){
            cout<<"Server Crash"<<endl;
            break;
        }
        ret=send_n(socketfd,buf,dataLen);
        if(-1==ret){
            cout<<"Server Crash"<<endl;
            break;
        }
        splitChar(buf,operate,operand);
        
        if(strcmp(operate,"cd")==0){
            continue;
        }
        else if(strcmp(operate,"ls")==0){
            printf("type                         name                 size\n");
            while(1){
                //获取文件类型,f为文件,d为目录
                recv_n(socketfd,buf,1);
                if(buf[0]==0) break;
                printf("-%c",buf[0]);

                //获取文件名
                recv_n(socketfd,&dataLen,sizeof(int));
                recv_n(socketfd,buf,dataLen);
                buf[dataLen]=0;
                printf(" %30s",buf);

                //获取文件大小
                recv_n(socketfd,&fileSize,sizeof(long));

                printf("%20ldB\n",fileSize);
            }

        }
        else if(strcmp(operate,"puts")==0){
            //传文件名
            ret=transFile(socketfd,operand);
            if(0==ret) cout<<"puts success"<<endl;
            else if(-1==ret) perror("puts failed:");
        }
        else if(strcmp(operate,"gets")==0){

        }
        else if(strcmp(operate,"remove")==0){

        }
        else if(strcmp(operate,"pwd")==0){
            recv_n(socketfd,&dataLen,sizeof(int));
            ret=recv_n(socketfd,buf,dataLen);
            cout<<buf<<endl;
        }

    }

    close(socketfd);
    return 0;
}

void splitChar(char* from,char* operate,char* operand){
    int fromLen=strlen(from);
    int i=0,j=0;
    while(from[i]==' '&&i<fromLen) i++;
    while(from[i]!=' '&&i<fromLen) operate[j++]=from[i++];
    while(from[i]==' '&&i<fromLen) i++;
    j=0;
    while(operate[i]!=' '&&i<fromLen) operand[j++]=from[i++];
}

