#include "thread_factory.h"

void ServerInit(char*,char*,char*,char*);//读配置文件
//四个参数为conf,ip,port,path,后三个为传出参数
void* threadFunc(void*);//子线程操作
void ServerClient(void*);//为客户端服务
void splitChar(char*,char*,char*);//分割char*,将中间空格分开,后两个参数为传出参数

int main(int argc,char* argv[])
{
    judgeArgc(argc,2);
    
    //读配置文件
    char ip[20]={0};
    char port[10]={0};
    char absPath[64]={0};
    ServerInit(argv[1],ip,port,absPath);

    //创建子线程
    int i;
    factory f;
    memset(&f,0,sizeof(f));
    f.pthid=(pthread_t*)calloc(ThreadNum,sizeof(pthread_t));
    f.threadQue=(pQue)calloc(1,sizeof(Que));
    f.threadQue->Capacity=MaxNum;
    pthread_mutex_init(&f.threadQue->queMutex,NULL);
    pthread_cond_init(&f.cond,NULL);
    f.state=1;//1为线程池开启
    for(i=0;i<ThreadNum;i++){
        pthread_create(&f.pthid[i],NULL,threadFunc,&f);
    }
    cout<<"thread pool start success"<<endl;
    
    //tcp初始化
    int socketfd=tcpInit(ip,port);
    listen(socketfd,MaxNum);

    //初始化epoll
    int epfd=epoll_create(1);
    struct epoll_event event,evs[MaxNum+1];
    memset(&event,0,sizeof(event));
    event.events=EPOLLIN;
    event.data.fd=socketfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,socketfd,&event);
    int waitNum,newfd,ret;
    pNode pnode;
    struct sockaddr_in sock;
    memset(&sock,0,sizeof(sockaddr_in));
    socklen_t addrlen=sizeof(sock);

    while(1){
        waitNum=epoll_wait(epfd,evs,MaxNum+1,-1);
        for(i=0;i<waitNum;i++){
            //如果描述符是socketfd
            if(evs[i].data.fd==socketfd){
                newfd=accept(socketfd,(struct sockaddr*)&sock,&addrlen);
                printf("client %s %d connect\n",inet_ntoa(sock.sin_addr),ntohs(sock.sin_port));
                pnode=(pNode)calloc(1,sizeof(Node));
                pnode->newfd=newfd;
                pnode->pUserInfo=(pUserCtl)calloc(1,sizeof(UserCtl));
                strcpy(pnode->pUserInfo->absPath,absPath);
                pthread_mutex_lock(&f.threadQue->queMutex);
                ret=queInsert(f.threadQue,pnode);
                pthread_mutex_unlock(&f.threadQue->queMutex);
                if(-1==ret){//队满直接关闭
                    close(newfd);
                    free(pnode);
                    pnode=NULL;
                }
                pthread_cond_signal(&f.cond);
            }
        }
    }

    close(socketfd);
    return 0;
}

void* threadFunc(void* p){
    pFactory pf=(pFactory)p;
    int ret;
    pNode pnode;
    while(1){
        if(pf->state==0){
            pthread_exit(0);
        }
        pthread_mutex_lock(&pf->threadQue->queMutex);
        if(0==pf->threadQue->currentSize){
            pthread_cond_wait(&pf->cond,&pf->threadQue->queMutex);
        }
        ret=quePop(pf->threadQue,&pnode);
        pthread_mutex_unlock(&pf->threadQue->queMutex);
        if(0==ret){
            //ret为0则成功从队列取出结点,子线程开始为客户端服务
            
            ServerClient(pnode);
            cout<<"Client bye"<<endl;
            close(pnode->newfd);
        }
    }
}

//子线程服务客户端
void ServerClient(void* p){
    char buf[128]={0};
    char buf1[128]={0};
    char operate[32]={0};//操作类型
    char operand[96]={0};//操作对象
    pNode pnode=(pNode)p;
    int ret,i,dataLen,fd;
    
    DIR *dir;
    struct dirent *dnt;
    struct stat sta;

    while(1){
        memset(buf,0,sizeof(buf));
        memset(operate,0,sizeof(operate));
        memset(operand,0,sizeof(operand));
        ret=recv_n(pnode->newfd,&dataLen,sizeof(int));
        if(-1==ret) return;
        ret=recv_n(pnode->newfd,buf,dataLen);
        if(-1==ret) return;

        //将操作数和对象分开
        splitChar(buf,operate,operand);
        
        if(strcmp(operate,"cd")==0){
            if(strcmp(operand,".")==0) continue;
            else if(strcmp(operand,"..")==0){
                if(strlen(pnode->pUserInfo->curPath)==0) continue;
                i=strlen(pnode->pUserInfo->curPath)-1;
                while(pnode->pUserInfo->curPath[i]!='/') i--;
                pnode->pUserInfo->curPath[i]=0;
            }
            else if(operand[0]=='/'){
                strcat(pnode->pUserInfo->curPath,operand);
            }
            else{
                pnode->pUserInfo->curPath[strlen(pnode->pUserInfo->curPath)+1]=0;
                pnode->pUserInfo->curPath[strlen(pnode->pUserInfo->curPath)]='/';
                strcat(pnode->pUserInfo->curPath,operand);
            }
        }
        else if(strcmp(operate,"ls")==0){
            strcpy(buf,pnode->pUserInfo->absPath);
            strcat(buf,pnode->pUserInfo->curPath);
            dir=opendir(buf);
            i=strlen(buf)+1;
            buf[i-1]='/';
            while((dnt=readdir(dir))!=NULL){
                if(dnt->d_name[0]=='.'||!strcmp("..",dnt->d_name)||!strcmp(".",dnt->d_name)) continue;
                buf[i]=0;
                strcat(buf,dnt->d_name);
                stat(buf,&sta);
                
                //传文件类型
                ret=sta.st_mode>>12;
                if(8==ret){//8为文件
                    buf1[0]='f';
                    send_n(pnode->newfd,buf1,1);
                }
                else if(4==ret){//4为目录
                    buf1[0]='d';
                    send_n(pnode->newfd,buf1,1);
                }

                //传文件名
                strcpy(buf1,dnt->d_name);
                dataLen=strlen(buf1);
                send_n(pnode->newfd,&dataLen,sizeof(int));
                send_n(pnode->newfd,buf1,dataLen);

                //传文件大小
                send_n(pnode->newfd,&sta.st_size,sizeof(long));
            }
            dataLen=0;
            send_n(pnode->newfd,&dataLen,1);
            closedir(dir);
            dir=NULL;
        }
        else if(strcmp(operate,"puts")==0){
            strcpy(buf,pnode->pUserInfo->absPath);
            strcat(buf,pnode->pUserInfo->curPath);
            ret=recvFile(pnode->newfd,buf);
            if(0==ret) cout<<"puts success"<<endl;
            else if(-1==ret) perror("puts failed");
        }
        else if(strcmp(operate,"gets")==0){
            strcpy(buf,pnode->pUserInfo->absPath);
            strcat(buf,pnode->pUserInfo->curPath);
            buf[strlen(buf)+1]=0;
            buf[strlen(buf)]='/';
            strcat(buf,operand);
            ret=transFile(pnode->newfd,buf);
            if(0==ret) cout<<"gets success"<<endl;
            else if(-1==ret) perror("gets failed");
        }
        else if(strcmp(operate,"remove")==0){
            strcpy(buf,pnode->pUserInfo->absPath);
            strcat(buf,pnode->pUserInfo->curPath);
            buf[strlen(buf)+1]=0;
            buf[strlen(buf)]='/';
            strcat(buf,operand);
            remove(buf);
        }
        else if(strcmp(operate,"pwd")==0){
            strcpy(buf,pnode->pUserInfo->absPath);
            strcat(buf,pnode->pUserInfo->curPath);
            dataLen=strlen(buf);
            send_n(pnode->newfd,&dataLen,sizeof(int));
            send_n(pnode->newfd,buf,dataLen);
        }

    }
}

//将from中间的空格分开,前部分为操作数,后部分为操作对象
void splitChar(char* from,char* operate,char* operand){
    int fromLen=strlen(from);
    int i=0,j=0;
    while(from[i]==' '&&i<fromLen) i++;
    while(from[i]!=' '&&i<fromLen) operate[j++]=from[i++];
    while(from[i]==' '&&i<fromLen) i++;
    j=0;
    while(operate[i]!=' '&&i<fromLen) operand[j++]=from[i++];
}

void ServerInit(char* conf,char* ip,char* port,char* absPath){
    int fd=open(conf,O_RDONLY);
    char buf[128]={0};
    read(fd,buf,sizeof(buf));
    close(fd);
    int cnt=0;
    int start=0;
    while(buf[start+cnt]!='\n'){
        cnt++;
    }
    strncpy(ip,buf,cnt);
    start=start+cnt+1;
    cnt=0;
    while(buf[start+cnt]!='\n'){
        cnt++;
    }
    strncpy(port,buf+start,cnt);
    start=start+cnt+1;
    while(buf[start+cnt]!='\n'){
        cnt++;
    }
    strncpy(absPath,buf+start,cnt);
}
