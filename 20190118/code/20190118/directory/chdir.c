#include "func.h"

int main()
{
	char path[128]={0};
	char *pret;
	pret=getcwd(path,sizeof(path));
	if(NULL==pret)
	{
		perror("getcwd");
		return -1;
	}
	printf("%s\n",path);
	chdir("..");
	memset(path,0,sizeof(path));
	pret=getcwd(NULL,0);
	printf("%s\n",pret);
	return 0;
}

