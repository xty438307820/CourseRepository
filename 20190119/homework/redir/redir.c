#include "func.h"

int main(int argc,char* argv[]){
	judgeArgc(argc,3);
	int fd1=open(argv[1],O_RDWR);
	if(-1==fd1){
		perror("open");
		return -1;
	}
	int fd2=open(argv[2],O_RDWR);
	if(-1==fd2){
		perror("open");
		return -1;
	}
	int c1=dup2(fd1,STDOUT_FILENO);
	int c2=dup2(fd2,STDERR_FILENO);
	printf("hello");
	write(c2,"world",5);
	return 0;
}
