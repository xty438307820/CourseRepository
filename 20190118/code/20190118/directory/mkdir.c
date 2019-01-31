#include "func.h"

int main(int argc,char* argv[])
{
	args_check(argc,2);
	int ret;
	ret=mkdir(argv[1],0777);
	if(-1==ret)
	{
		perror("mkdir");
		return -1;
	}
	return 0;
}
