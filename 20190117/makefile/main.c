#include<stdio.h>
#include<stdlib.h>

int mul(int a,int b){
	printf("1:%d\n",a*b);
	return a*b;
}

int main(int argv,char* argc[]){
	int x,y;
	scanf("%d%d",&x,&y);
	printf("2:%d\n",mul(x,y));
	return 0;
}
