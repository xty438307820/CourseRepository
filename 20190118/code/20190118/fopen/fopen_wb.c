#include <stdio.h>

int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		printf("./ELF filename\n");
		return -1;
	}
	FILE *fp;
	fp=fopen(argv[1],"wb");
	if(NULL==fp)
	{
		perror("fopen");
		return -1;
	}
	fclose(fp);
	return 0;
}
