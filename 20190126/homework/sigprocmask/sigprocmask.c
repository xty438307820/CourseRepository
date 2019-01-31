#include<func.h>

int main(){	
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set,SIGINT);

	sigprocmask(SIG_BLOCK,&set,NULL);
	sleep(5);
	sigpending(&set);
	if(sigismember(&set,SIGINT)){
		printf("SIGINT is existing\n");
	}
	else{
		printf("SIGINT isn't existing\n");
	}
	sigemptyset(&set);
	sigaddset(&set,SIGINT);
	sigprocmask(SIG_UNBLOCK,&set,NULL);
	return 0;
}
