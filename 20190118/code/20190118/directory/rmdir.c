#include "func.h"

int main(int argc,char* argv[])
{
	args_check(argc,2);
	int ret;
	ret=rmdir(argv[1]);
	if(-1==ret)
	{
		perror("rmdir");
		return -1;
	}
	return 0;
}
