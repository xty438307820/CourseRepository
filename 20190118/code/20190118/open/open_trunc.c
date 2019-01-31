#include "func.h"

int main(int argc,char **argv)
{
	args_check(argc,2);
	int fd;
	fd=open(argv[1],O_RDWR|O_TRUNC);
	if(-1==fd)
	{
		perror("open");
		return -1;
	}
	printf("fd=%d\n",fd);
	return 0;
}
