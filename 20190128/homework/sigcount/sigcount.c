#include<func.h>

void printtime(int snum){
	time_t t;
	t=time(NULL);
	printf("%s",ctime(&t));
}

int main(){
	//signal(SIGALRM,printtime);
	signal(SIGPROF,printtime);
	//signal(SIGVTALRM,printtime);
	struct itimerval it;
	memset(&it,0,sizeof(it));
	it.it_interval.tv_sec=2;
	it.it_value.tv_sec=4;
	printtime(0);
	//setitimer(ITIMER_REAL,&it,NULL);
	setitimer(ITIMER_PROF,&it,NULL);
	//setitimer(ITIMER_VIRTUAL,&it,NULL);
	sleep(3);
	while(1);
	return 0;
}
