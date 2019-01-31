#include "func.h"

int main(){
	FILE *fp=popen("./print","r");
	if(NULL==fp){
		perror("popen");
		return -1;
	}
	printf("I am parent\n");
	char buf[128]={0};
	fread(buf,1,sizeof(buf),fp);
	printf("%s",buf);
	/*while(fgets(buf,sizeof(buf),fp)){
		printf("%s",buf);
	}*/
	pclose(fp);
	return 0;
}
