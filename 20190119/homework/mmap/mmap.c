#include "func.h"

int main(int argc,char* argv[]){
	judgeArgc(argc,2);
	int fd=open(argv[1],O_RDWR);
	struct stat sta;
	char *c;
	fstat(fd,&sta);
	c=mmap(NULL,sta.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	strcpy(c,"world");
	munmap(c,sta.st_size);
	return 0;
}
