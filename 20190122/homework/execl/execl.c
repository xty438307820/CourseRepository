#include "func.h"

int main(){
	execl("b.out","b.exe","abc","def",NULL);
	printf("I am a.out\n");
	return 0;
}
