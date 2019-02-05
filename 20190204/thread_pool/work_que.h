#ifndef __WORK_QUE__
#define __WORK_QUE__
#include "func_pool.h"

typedef struct Node{
    int newfd;//与客户端传数据的socketfd
    struct Node* nextNode;
}Node,*pNode;

//队列结构体，实现先来先服务
typedef struct{
    pNode phead,pTail;
    int currentSize;
    int Capacity;
    pthread_mutex_t queMutex;
}Que,*pQue;

void queInsert(pQue,pNode);//入队
void quePop(pQue,pNode*);//出队

#endif
