#include "func.h"

int main(int argc,char *argv[])
{
	args_check(argc,2);
	DIR *pdir;
	pdir=opendir(argv[1]);
	if(NULL==pdir)
	{
		perror("opendir");
		return -1;
	}
	struct dirent *p;
	while((p=readdir(pdir))!=NULL)
	{
		printf("d_ino=%ld,d_reclen=%d,d_type=%d,d_name=%s\n",p->d_ino,p->d_reclen,p->d_type,p->d_name);
	}
	closedir(pdir);
	return 0;
}
