#include "func.h"

int main(){
	char buf[128]={0};
	int fd=open("file",O_RDWR);
	if(fork()>0){
		lseek(fd,2,SEEK_SET);
		return 0;
	}
	sleep(2);
	if(read(fd,buf,sizeof(buf))<1){
		perror("read");
	}
	else printf("%s\n",buf);
	close(fd);
	return 0;
}
