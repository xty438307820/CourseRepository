#include <stdio.h>

typedef struct{
	int num;
	char name[20];
	float score;
}stu;
int main()
{
	stu s={1001,"Lili",99.5};
	char buf[128]={0};
	sprintf(buf,"%d %s %5.2f",s.num,s.name,s.score);
	puts(buf);
	return 0;
}

