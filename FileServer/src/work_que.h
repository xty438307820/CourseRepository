#ifndef __WORK_QUE__
#define __WORK_QUE__
#include "funcpp.h"

typedef struct{
    char absPath[64];//home路径
    char curPath[192];//相对路径
    char UserId[30];//用户Id
    char UserName[30];//用户名
}UserCtl,*pUserCtl;

typedef struct Node{
    int newfd;//与客户端传数据的socketfd
    pUserCtl pUserInfo;
    struct Node* nextNode;
}Node,*pNode;

//队列结构体，实现先来先服务
typedef struct{
    pNode phead,pTail;
    int currentSize;
    int Capacity;//最大容量100
    pthread_mutex_t queMutex;
}Que,*pQue;

int queInsert(pQue,pNode);//入队
int quePop(pQue,pNode*);//出队
int queEmpty(pQue);

#endif
