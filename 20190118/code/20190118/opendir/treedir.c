#include "func.h"
//目录深度优先遍历
void printdir(char * dirname,int width)
{
	DIR *pdir;
	pdir=opendir(dirname);
	if(NULL==pdir)
	{
		perror("opendir");
		return;
	}
	struct dirent *p;
	char path[512]={0};
	while((p=readdir(pdir))!=NULL)
	{
		if(!strcmp(p->d_name,".")||!strcmp(p->d_name,"..")) 
		{
			continue;
		}
		printf("%*s%s\n",width,"",p->d_name);
		if(p->d_type==4)
		{
			sprintf(path,"%s%s%s",dirname,"/",p->d_name);
			printdir(path,width+4);
		}
	}
	closedir(pdir);
}

int main(int argc,char *argv[])
{
	args_check(argc,2);
	printf("%s\n",argv[1]);
	printdir(argv[1],4);
	return 0;
}
