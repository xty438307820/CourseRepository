#include "func.h"

int main(){
	FILE *fp=popen("./scanf","w");
	fwrite("hello",1,5,fp);
	sleep(3);
	fwrite("world\n",1,6,fp);
	printf("I am parent\n");
	pclose(fp);
	return 0;
}
