#include "func.h"

int main(){
	FILE *fp=fopen("file","r");
	char buf[128]={0};
	while(fgets(buf,sizeof(buf),fp)){
		printf("%s",buf);
		memset(buf,0,sizeof(buf));
	}
	fclose(fp);
	return 0;
}
