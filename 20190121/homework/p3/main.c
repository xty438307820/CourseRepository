#include "func.h"

int main(int argc,char* argv[]){
	judgeArgc(argc,2);
	int fd=open(argv[1],O_RDWR);
	if(-1==fd){
		perror("open");
		return -1;
	}
	pid_t uid=getuid();
	pid_t gid=getgid();
	pid_t euid=geteuid();
	pid_t egid=getegid();
	printf("uid=%d,gid=%d,euid=%d,egid=%d\n",uid,gid,euid,egid);
	write(fd,"hello",5);
	return 0;
}
