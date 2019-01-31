#include<stdio.h>

int main(){

#ifdef arg
	printf("Def\n");
#else
	printf("Not def\n");
#endif
	return 0;
}
