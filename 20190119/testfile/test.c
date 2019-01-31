#include "func.h"

int main(int argc,char* argv[]){
	judgeArgc(argc,2);
	int fd=open(argv[1],O_RDWR);
	FILE *fp1=fdopen(fd,"rb+");
	FILE *fp2=fdopen(fd,"rb+");
	FILE *fp3=fdopen(fd,"rb+");
	printf("%d %d %d\n",fp1,fp2,fp3);	
	lseek(fd,5,SEEK_SET);
	fwrite("222",1,3,fp2);
	fwrite("333",1,3,fp3);
	fwrite("111",1,3,fp1);
	fflush(fp1);
	write(fd,"444",3);
	return 0;
}
