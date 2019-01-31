#include "func.h"

int main(int argc,char* argv[]){
	judgeArgc(argc,2);
	int fd=open(argv[1],O_WRONLY);
	if(-1==fd){
		perror("open");
		return -1;
	}
	printf("I am writer\n");
	sleep(5);
	write(fd,"ddd\n",4);
	return 0;
}
