#include "func.h"

int main(int argc,char **argv)
{
	args_check(argc,2);
	int fd;
	fd=open(argv[1],O_RDWR|O_CREAT,0777);
	if(-1==fd)
	{
		perror("open");
		return -1;
	}
	printf("fd=%d\n",fd);
	return 0;
}
