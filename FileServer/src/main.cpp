#include "thread_factory.h"

void ServerInit(char*,char*,char*,char*);//读配置文件
//四个参数为conf,ip,port,path,后三个为传出参数
void* threadFunc(void*);//子线程操作
void ServerClient(void*);//为客户端服务
void splitChar(char*,char*,char*);//分割char*,将中间空格分开,后两个参数为传出参数
int fileExist(char*,char*,char);//根据传入的绝对路径判断文件或文件夹是否存在,用于cd和gets,存在返回1,否则返回0

int logfd;//日志记录fd

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
    logfd=open("../bin/log",O_WRONLY|O_APPEND|O_CREAT,0666);
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
    char buf[128]={0},bufTim[64]={0};
    time_t tim;

    while(1){
        waitNum=epoll_wait(epfd,evs,MaxNum+1,-1);
        for(i=0;i<waitNum;i++){
            //如果描述符是socketfd
            if(evs[i].data.fd==socketfd){
                newfd=accept(socketfd,(struct sockaddr*)&sock,&addrlen);
                printf("client %s %d connect\n",inet_ntoa(sock.sin_addr),ntohs(sock.sin_port));
                tim=time(NULL);
                ctime_r(&tim,bufTim);
                sprintf(buf,"client %s %d connect %s",inet_ntoa(sock.sin_addr),ntohs(sock.sin_port),bufTim);
                write(logfd,buf,strlen(buf));
                pnode=(pNode)calloc(1,sizeof(Node));
                pnode->newfd=newfd;
                pnode->pUserInfo=(pUserCtl)calloc(1,sizeof(UserCtl));
                strcpy(pnode->pUserInfo->absPath,absPath);
                strcpy(pnode->pUserInfo->Virtual,VirtualPath);
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

    close(logfd);
    close(socketfd);
    return 0;
}

void* threadFunc(void* p){
    pFactory pf=(pFactory)p;
    int ret,dataLen;
    pNode pnode;
    char buf[128]={0};
    char buf1[128]={0};
    char UserName[32]={0};
    char sql[216]={0};
    char salt[32]={0};
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
begin:
            recv_n(pnode->newfd,&ret,sizeof(int));
            if(1==ret){//登录
                memset(UserName,0,sizeof(UserName));
                memset(salt,0,sizeof(salt));
                recv_n(pnode->newfd,&dataLen,sizeof(int));
                recv_n(pnode->newfd,&UserName,dataLen);

                sprintf(sql,"select Salt from UserInfo where UserName='%s'",UserName);
                sqlSingleSelect(sql,salt);
                if(strlen(salt)==0) strcpy(salt,"$6$sss");
                dataLen=strlen(salt);
                send_n(pnode->newfd,&dataLen,sizeof(int));
                send_n(pnode->newfd,salt,dataLen);
                memset(buf,0,sizeof(buf));
                recv_n(pnode->newfd,&dataLen,sizeof(int));
                recv_n(pnode->newfd,buf,dataLen);

                sprintf(sql,"select PassWord from UserInfo where UserName='%s'",UserName);
                sqlSingleSelect(sql,buf1);

                if(strcmp(buf,buf1)==0){
                    ret=1;
                    send_n(pnode->newfd,&ret,sizeof(int));
                    pnode->pUserInfo->absPath[strlen(pnode->pUserInfo->absPath)]='/';
                    strcat(pnode->pUserInfo->absPath,UserName);
                    strcpy(pnode->pUserInfo->UserName,UserName);
                }
                else{
                    ret=0;
                    send_n(pnode->newfd,&ret,sizeof(int));
                    recv_n(pnode->newfd,&dataLen,sizeof(int));
                    memset(buf,0,sizeof(buf));
                    recv_n(pnode->newfd,buf,dataLen);
                    if(strcmp(buf,buf1)==0){
                        ret=1;
                        send_n(pnode->newfd,&ret,sizeof(int));
                        pnode->pUserInfo->absPath[strlen(pnode->pUserInfo->absPath)]='/';
                        strcat(pnode->pUserInfo->absPath,UserName);
                        strcpy(pnode->pUserInfo->UserName,UserName);
                    }
                    else{
                        ret=0;
                        send_n(pnode->newfd,&ret,sizeof(int));
                        close(pnode->newfd);
                        continue;
                    }
                }
            }
            else if(2==ret){//注册

                //用户名
                recv_n(pnode->newfd,&dataLen,sizeof(int));
                recv_n(pnode->newfd,buf,dataLen);
                sprintf(sql,"select * from UserInfo where UserName='%s'",buf);
                ret=sqlFindData(sql);
                while(ret){
                    send_n(pnode->newfd,&ret,sizeof(int));
                    memset(buf,0,sizeof(buf));
                    recv_n(pnode->newfd,&dataLen,sizeof(int));
                    recv_n(pnode->newfd,buf,dataLen);
                    sprintf(sql,"select UserName from UserInfo where UserName='%s'",buf);
                    ret=sqlFindData(sql);
                }
                send_n(pnode->newfd,&ret,sizeof(int));
                strcpy(UserName,buf);//用户名输入结束

                //密码
                memset(buf,0,sizeof(buf));
                recv_n(pnode->newfd,&dataLen,sizeof(int));
                recv_n(pnode->newfd,salt,dataLen);
                recv_n(pnode->newfd,&dataLen,sizeof(int));
                recv_n(pnode->newfd,buf,dataLen);

                sprintf(sql,"insert into UserInfo(UserName,Salt,PassWord) values('%s','%s','%s')",UserName,salt,buf);
                sqlTableChange(sql);
                memset(buf,0,sizeof(buf));
                strcpy(buf,pnode->pUserInfo->absPath);
                buf[strlen(buf)]='/';
                strcat(buf,UserName);
                mkdir(buf,0775);
                goto begin;
            }
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
    char fileName[128]={0};
    char operate[32]={0};//操作类型
    char operand[96]={0};//操作对象
    char *secret,*pwd;
    char sql[216]={0};
    pNode pnode=(pNode)p;
    int ret,i,dataLen,fd;
    
    DIR *dir;
    struct dirent *dnt;
    struct stat sta;
    long fileSize;

    time_t tim;
    tim=time(NULL);

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
            ctime_r(&tim,buf1);
            sprintf(buf,"%s: %s %s %s",pnode->pUserInfo->UserName,operate,operand,buf1);
            write(logfd,buf,strlen(buf));
            if(strcmp(operand,".")==0){
                ret=1;
                send_n(pnode->newfd,&ret,sizeof(int));
                continue;
            }
            else if(strcmp(operand,"..")==0){
                if(strlen(pnode->pUserInfo->curPath)==0){
                    ret=1;
                    send_n(pnode->newfd,&ret,sizeof(int));
                    continue;
                }
                i=strlen(pnode->pUserInfo->curPath)-1;
                while(i>=0&&pnode->pUserInfo->curPath[i]!='/') i--;
                if(i>=0) pnode->pUserInfo->curPath[i]=0;
            }
            else if(operand[0]=='/'){
                strcat(pnode->pUserInfo->curPath,operand);
            }
            else{
                pnode->pUserInfo->curPath[strlen(pnode->pUserInfo->curPath)+1]=0;
                strcpy(buf,pnode->pUserInfo->absPath);
                strcat(buf,pnode->pUserInfo->curPath);
                ret=fileExist(buf,operand,'d');
                if(0==ret){
                    send_n(pnode->newfd,&ret,sizeof(int));
                    continue;
                }
                pnode->pUserInfo->curPath[strlen(pnode->pUserInfo->curPath)]='/';
                strcat(pnode->pUserInfo->curPath,operand);
            }
            ret=1;
            send_n(pnode->newfd,&ret,sizeof(int));
        }
        else if(strcmp(operate,"ls")==0){
            ctime_r(&tim,buf1);
            sprintf(buf,"%s: %s %s",pnode->pUserInfo->UserName,operate,buf1);
            write(logfd,buf,strlen(buf));
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
                    buf1[0]='-';
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
            ctime_r(&tim,buf1);
            sprintf(buf,"%s: %s %s %s",pnode->pUserInfo->UserName,operate,operand,buf1);
            write(logfd,buf,strlen(buf));
            strcpy(buf,pnode->pUserInfo->absPath);
            strcat(buf,pnode->pUserInfo->curPath);
            recv_n(pnode->newfd,&ret,sizeof(int));
            if(-1==ret){
                continue;
            }
            memset(fileName,0,sizeof(fileName));
            recv_n(pnode->newfd,&dataLen,sizeof(int));
            recv_n(pnode->newfd,fileName,dataLen);//接文件名

            memset(buf1,0,sizeof(buf1));
            recv_n(pnode->newfd,&dataLen,sizeof(int));
            recv_n(pnode->newfd,buf1,dataLen);
            sprintf(sql,"select FileName from FileInfo where FileName='%s'",buf1);
            ret=sqlFindData(sql);
            if(ret==1){//虚拟目录存在,执行秒传
                ret=2;
                send_n(pnode->newfd,&ret,sizeof(int));
                strcpy(buf,pnode->pUserInfo->Virtual);
                buf[strlen(buf)+1]=0;
                buf[strlen(buf)]='/';
                strcat(buf,buf1);
                strcpy(buf1,pnode->pUserInfo->absPath);
                strcat(buf1,pnode->pUserInfo->curPath);
                buf1[strlen(buf1)+1]=0;
                buf1[strlen(buf1)]='/';
                strcat(buf1,fileName);
                link(buf,buf1);//建立硬链接
                continue;
            }
            ret=fileExist(buf,operand,'d');
            if(1==ret){//存在相同名的文件夹，不能puts
                ret=-1;
                send_n(pnode->newfd,&ret,sizeof(int));
                continue;
            }
            ret=fileExist(buf,operand,'-');
            if(1==ret){//文件已经存在,执行断点续传
                send_n(pnode->newfd,&ret,sizeof(int));
                i=strlen(buf);
                buf[strlen(buf)+1]=0;
                buf[strlen(buf)]='/';
                strcat(buf,operand);
                stat(buf,&sta);
                send_n(pnode->newfd,&sta.st_size,sizeof(long));
                fileSize=sta.st_size;
                buf[i]=0;
            }
            else if(0==ret){
                fileSize=0;
                send_n(pnode->newfd,&ret,sizeof(int));
            }
            ret=recvFile(pnode->newfd,buf,fileSize);
            if(0==ret){
                sprintf(sql,"insert into FileInfo values('%s')",buf1);
                strcpy(buf,pnode->pUserInfo->Virtual);
                buf[strlen(buf)+1]=0;
                buf[strlen(buf)]='/';
                strcat(buf,buf1);//buf为虚拟目录路径
                strcpy(buf1,pnode->pUserInfo->absPath);
                strcat(buf1,pnode->pUserInfo->curPath);
                buf1[strlen(buf1)+1]=0;
                buf1[strlen(buf1)]='/';
                strcat(buf1,fileName);//buf1为用户空间目录路径
                link(buf1,buf);//建立硬链接
                ret=sqlTableChange(sql);
            }
            else if(-1==ret) perror("puts failed");
        }
        else if(strcmp(operate,"gets")==0){
            ctime_r(&tim,buf1);
            sprintf(buf,"%s: %s %s %s",pnode->pUserInfo->UserName,operate,operand,buf1);
            write(logfd,buf,strlen(buf));
            strcpy(buf,pnode->pUserInfo->absPath);
            strcat(buf,pnode->pUserInfo->curPath);
            ret=fileExist(buf,operand,'-');
            if(0==ret){//文件不存在,gets失败
                ret=-1;
                send_n(pnode->newfd,&ret,sizeof(int));
                continue;
            }
            else if(ret==1){//文件存在
                send_n(pnode->newfd,&ret,sizeof(int));
            }
            recv_n(pnode->newfd,&ret,sizeof(int));
            if(1==ret){//文件存在,进行断点续传
                recv_n(pnode->newfd,&fileSize,sizeof(long));
            }
            else{
                fileSize=0;
            }
            buf[strlen(buf)+1]=0;
            buf[strlen(buf)]='/';
            strcat(buf,operand);
            ret=transFile(pnode->newfd,buf,fileSize);
            if(0==ret) cout<<"gets success"<<endl;
            else if(-1==ret) perror("gets failed");
        }
        else if(strcmp(operate,"remove")==0){
            ctime_r(&tim,buf1);
            sprintf(buf,"%s: %s %s %s",pnode->pUserInfo->UserName,operate,operand,buf1);
            write(logfd,buf,strlen(buf));
            strcpy(buf,pnode->pUserInfo->absPath);
            strcat(buf,pnode->pUserInfo->curPath);
            ret=fileExist(buf,operand,'d');
            if(1==ret){//删除文件夹
                send_n(pnode->newfd,&ret,sizeof(int));
                buf[strlen(buf)+1]=0;
                buf[strlen(buf)]='/';
                strcat(buf,operand);
                remove(buf);
                continue;
            }
            ret=fileExist(buf,operand,'-');
            if(1==ret){//文件存在
                send_n(pnode->newfd,&ret,sizeof(int));
                buf[strlen(buf)+1]=0;
                buf[strlen(buf)]='/';
                strcat(buf,operand);
                stat(buf,&sta);
                if(sta.st_nlink>2){
                    remove(buf);
                }
                else{//硬链接数=2,删除用户目录文件、虚拟目录文件
                    fd=open(buf,O_RDONLY);
                    pwd=(char*)mmap(NULL,sta.st_size,PROT_READ,MAP_SHARED,fd,0);
                    secret=(char*)MD5((unsigned char*)pwd,sta.st_size,NULL);
                    munmap(pwd,sta.st_size);
                    close(fd);
                    
                    strcpy(buf1,VirtualPath);
                    buf1[strlen(buf1)+1]=0;
                    buf1[strlen(buf1)]='/';
                    strcat(buf1,secret);
                    sprintf(sql,"delete from FileInfo where FileName='%s'",secret);
                    sqlTableChange(sql);
                    remove(buf);
                    remove(buf1);
                }
                continue;
            }
            else if(0==ret){//文件或文件夹不存在
                ret=-1;
                send_n(pnode->newfd,&ret,sizeof(int));
                continue;
            }
        }
        else if(strcmp(operate,"pwd")==0){
            ctime_r(&tim,buf1);
            sprintf(buf,"%s: %s %s",pnode->pUserInfo->UserName,operate,buf1);
            write(logfd,buf,strlen(buf));
            memset(buf,0,sizeof(buf));
            strcpy(buf,pnode->pUserInfo->curPath);
            if(strlen(buf)==0){
                buf[0]='/';
            }
            dataLen=strlen(buf);
            send_n(pnode->newfd,&dataLen,sizeof(int));
            send_n(pnode->newfd,buf,dataLen);
        }
        else if(strcmp(operate,"mkdir")==0){
            ctime_r(&tim,buf1);
            sprintf(buf,"%s: %s %s",pnode->pUserInfo->UserName,operate,buf1);
            write(logfd,buf,strlen(buf));
            strcpy(buf,pnode->pUserInfo->absPath);
            strcat(buf,pnode->pUserInfo->curPath);
            ret=fileExist(buf,operand,'d');
            if(1==ret) ret=-1;
            if(0==ret) ret=fileExist(buf,operand,'-');
            if(1==ret) ret=-1;
            send_n(pnode->newfd,&ret,sizeof(int));
            if(0==ret){
                buf[strlen(buf)+1]=0;
                buf[strlen(buf)]='/';
                strcat(buf,operand);
                mkdir(buf,0775);
            }
        }
    }
}

//绝对路径如/home/adam，最后一个文件夹后面无'/'
int fileExist(char* absPath,char* fileName,char fileType){
    DIR *dir=opendir(absPath);
    struct dirent *dnt;
    char buf[128]={0};
    strcpy(buf,absPath);
    buf[strlen(buf)]='/';
    int i=strlen(buf);
    struct stat sta;
    char type;
    while((dnt=readdir(dir))!=NULL){
        buf[i]=0;
        strcat(buf,dnt->d_name);
        stat(buf,&sta);
        if(sta.st_mode>>12==4){//4是目录
            type='d';
        }
        else if(sta.st_mode>>12==8){//8是文件
            type='-';
        }
        if(strcmp(dnt->d_name,fileName)==0&&fileType==type){
            closedir(dir);
            return 1;
        }
    }
    closedir(dir);
    return 0;
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
