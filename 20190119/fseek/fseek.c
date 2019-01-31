#include "func.h"

int main(int argc,char* argv[]){
	judgeArgc(argc,2);
	int fd=open(argv[1],O_RDWR);
	int fs=lseek(fd,100,SEEK_SET);
	printf("%d\n",fs);
	write(fd,"c",sizeof(char));
//	int flag=ftruncate(fd,30);
//	printf("%d\n",flag);
	return 0;
}
