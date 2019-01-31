#include "func.h"

void dfs(char* ch,int num){
	DIR *pdir=opendir(ch);
	if(NULL==pdir){
		perror("opendir");
		return;
	}
	struct dirent *p;
	char path[100]={0};
	while((p=readdir(pdir))!=NULL){
		if(!strcmp(".",p->d_name)||!strcmp("..",p->d_name)) continue;
		memset(path,0,sizeof(path));
		printf("%*s|—— %s\n",num-4,"",p->d_name);
		sprintf(path,"%s%s%s",ch,"/",p->d_name);
		if(p->d_type==4) dfs(path,num+4);
	}
}

int main(int argc,char* argv[]){
	judgeArgc(argc,2);
	printf("%s\n",argv[1]);
	dfs(argv[1],4);
	return 0;
}
