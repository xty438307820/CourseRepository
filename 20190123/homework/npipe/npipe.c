#include "func.h"

int main(){
	int fds[2];
	pipe(fds);
	int fk=fork();
	if(fk==0){
		close(fds[0]);
		write(fds[1],"Hello",5);
		close(fds[1]);
	}
	else{
		close(fds[1]);
		char buf[128]={0};
		wait(NULL);
		read(fds[0],buf,sizeof(buf));
		printf("%s\n",buf);
		close(fds[0]);
	}
	return 0;
}
