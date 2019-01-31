#include "func.h"

#define pe(ope,num) {if(-1==num){perror(ope);return -1;}}

typedef struct{
	char sid[20];
	char sname[20];
	float score;
}student,*pstudent;

void init(pstudent p){
	strcpy(p[0].sid,"1001");strcpy(p[0].sname,"xiongda");p[0].score=99.5;
	strcpy(p[1].sid,"1002");strcpy(p[1].sname,"xionger");p[1].score=90.6;
	strcpy(p[2].sid,"1003");strcpy(p[2].sname,"xiaoming");p[2].score=90;
}

int main(int argc,char* argv[]){
	judgeArgc(argc,2);
	student s[3];
	int i;
	init(s);
	int fd=open(argv[1],O_RDWR);
	pe("open",fd);
	char ch[100];
	for(i=0;i<3;i++){
		sprintf(ch,"%s %s %.2f",s[i].sid,s[i].sname,s[i].score);
		write(fd,ch,strlen(ch));
		write(fd,"\n",1);
	}
	lseek(fd,0,SEEK_SET);
	while(read(fd,ch,sizeof(ch))!=0){
		printf("%s",ch);
	}
	close(fd);
	return 0;
}
