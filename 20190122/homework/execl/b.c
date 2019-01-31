#include "func.h"

int main(int argc,char* argv[]){
	judgeArgc(argc,3);
	printf("%s %s %s\n",argv[0],argv[1],argv[2]);
	return 0;
}
