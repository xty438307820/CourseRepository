#include <stdio.h>

int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		printf("./ELF filename\n");
		return -1;
	}
	FILE *fp;
	fp=fopen(argv[1],"rb+");
	if(NULL==fp)
	{
		perror("fopen");
		return -1;
	}
	char buf[128]={0};
	fread(buf,sizeof(char),5,fp);
	printf("buf=%s\n",buf);
	long pos;
	pos=ftell(fp);
	printf("%ld\n",pos);
	fclose(fp);
	return 0;
}
