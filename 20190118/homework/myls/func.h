#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<dirent.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>

#define judgeArgc(argc,num) {if(argc!=num){printf("arg err!\n");return -1;}}
