#include<func.h>

void fun(int snum,siginfo_t *t,void *v){
	printf("%d is coming\n",snum);
	sleep(3);
	sigset_t set;
	sigemptyset(&set);
	sigpending(&set);
	if(sigismember(&set,SIGQUIT)){
		printf("SIGQUIT is existing\n");
	}
	else{
		printf("SIGQUIT isn't existing\n");
	}
	printf("%d after\n",snum);
}

int main(){
	struct sigaction sa;
	sa.sa_sigaction=fun;
	sa.sa_flags=SA_SIGINFO;
	sigaddset(&sa.sa_mask,SIGQUIT);
	sigaction(SIGINT,&sa,NULL);
	sigaction(SIGQUIT,&sa,NULL);
	while(1);
	return 0;
}
