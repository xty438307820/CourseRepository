#ifndef __FUNCPP_H__
#define __FUNCPP_H__

#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<deque>
#include<list>
#include<set>
#include<map>
#include<stack>
#include<queue>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<dirent.h>
#include<time.h>
#include<sys/time.h>
#include<pwd.h>
#include<grp.h>
#include<fcntl.h>
#include<sys/select.h>
#include<sys/time.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/msg.h>
#include<signal.h>
#include<sys/msg.h>
#include<pthread.h>
#include<semaphore.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/epoll.h>
#include<sys/uio.h>
#include<sys/mman.h>
#include<mysql/mysql.h>
#include<shadow.h>
#include<openssl/md5.h>
using namespace std;

#define judgeArgc(argc,num) {if(argc!=num){printf("arg err!\n");return -1;}}
#endif
