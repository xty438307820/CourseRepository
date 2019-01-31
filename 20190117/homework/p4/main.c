#include<stdio.h>

int multi(int a,int b){
	return a*b;
}

int main(){
	int x=3;
	int y=4;
	int z=multi(x,y);
	printf("x*y=%d\n",z);
	return 0;
}
