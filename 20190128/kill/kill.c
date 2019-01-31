#include<func.h>

int main(int argc,char* argv[]){
	judgeArgc(argc,2);
	int pid=atoi(argv[1]);
	kill(pid,SIGINT);
	return 0;
}
