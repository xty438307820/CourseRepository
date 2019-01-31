#include<stdio.h>

int main(int argv,char* argc[]){

	prinf("%s\n",argc[0]);
	FILE *fp=fopen("test","rb+");
	if(NULL==fp){
		perror("fopen");
		return 0;
	}	
	return 0;
}
