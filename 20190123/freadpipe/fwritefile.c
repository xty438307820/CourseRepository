#include "func.h"

int main(){
	FILE *fp=fopen("file","w");
	fputs("hello\n",fp);
	sleep(5);
	fputs("world\n",fp);
	fclose(fp);
	return 0;
}
