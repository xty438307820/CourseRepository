#include <stdio.h>

typedef struct{
	int num;
	char name[20];
	float score;
}stu;
int main()
{
	stu s={0};
	char buf[128]="1001 Lili 99.5";
	sscanf(buf,"%d%s%f",&s.num,s.name,&s.score);
	printf("%d %s %5.2f\n",s.num,s.name,s.score);
	return 0;
}

