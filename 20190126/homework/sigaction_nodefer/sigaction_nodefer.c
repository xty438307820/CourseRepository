#include <func.h>

void fun(int snum,siginfo_t *t,void *v){
	printf("%d is coming\n",snum);
	sleep(3);
	printf("%d after\n",snum);
}

int main(){
	struct sigaction sa;
	sa.sa_sigaction=fun;
	sa.sa_flags=SA_SIGINFO|SA_NODEFER;
	sigaction(SIGINT,&sa,NULL);
	while(1);
	return 0;
}
