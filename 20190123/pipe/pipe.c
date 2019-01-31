#include "func.h"

int main(){
	int fds[2];
	pipe(fds);
	int fk=fork();
	if(0==fk){
		close(fds[0]);
		sleep(2);
		write(fds[1],"hello",5);
	}
	else{
		close(fds[1]);
		char buf[128]={0};
		read(fds[0],buf,sizeof(buf));
		printf("%s\n",buf);
		wait(NULL);
	}
	return 0;
}
