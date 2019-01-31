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
	return 0;
}

