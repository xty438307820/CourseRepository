#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#define args_check(argc,num) {if(argc!=num) {printf("error args\n");return -1;}}
