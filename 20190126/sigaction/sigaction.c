#include<func.h>

void sfunc(int snum,siginfo_t *t,void *v){
	printf("%d is coming\n",snum);
	sleep(2);
	printf("%d is after sleep\n",snum);
}

int main(){
	struct sigaction sa;
	sa.sa_flags=SA_SIGINFO|SA_RESTART|SA_NODEFER;
	sa.sa_sigaction=sfunc;
	sigaction(SIGINT,&sa,NULL);
	int i;
	scanf("%d",&i);
	printf("%d\n",i);
	return 0;
}
