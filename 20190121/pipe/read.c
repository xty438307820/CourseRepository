#include "func.h"

int main(int argc,char* argv[]){
	judgeArgc(argc,2);
	int fd=open(argv[1],O_RDONLY);
	if(-1==fd){
		perror("open");
		return -1;
	}
	printf("I am reader\n");
	char ch[30];
	int ret=read(fd,ch,sizeof(ch)-1);
	printf("ret=%d,ch=%s\n",ret,ch);
	return 0;
}
