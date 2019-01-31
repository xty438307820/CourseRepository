#include "func.h"

int main(){
	int fk=fork();
	if(0==fk){
		printf("son:pid=%d\n",getpid());
		printf("oldpgid=%d,oldsid=%d\n",getpgid(0),getsid(0));
		int sid=setsid();
		printf("newpgid=%d,newsid=%d\n",getpgid(0),getsid(0));
		chdir("/tmp");
		int fd=open("log",O_RDWR);
		while(1){
			sleep(1);
			write(fd,"I have sleep 1s\n",16);
		}
		close(fd);
	}
	else{
		printf("parent:pid=%d,pgid=%d\n",getpid(),getpgid(0));
	}
	return 0;
}
