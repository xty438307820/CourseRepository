#include "func.h"

int main(int argc,char **argv)
{
	args_check(argc,2);
	int ret;
	struct stat buf;
	ret=stat(argv[1],&buf);
	if(-1==ret)
	{
		perror("stat");
		return -1;
	}
	printf("%ld %x %ld %s %s %ld %ld %ld %s\n",buf.st_ino,buf.st_mode,buf.st_nlink,getpwuid(buf.st_uid)->pw_name,getgrgid(buf.st_gid)->gr_name,buf.st_size,buf.st_blksize,buf.st_blocks,ctime(&buf.st_mtime));
	return 0;
}

