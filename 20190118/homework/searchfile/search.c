#include "func.h"

void searchFile(char *dir,char *filename){
	DIR *pdir=opendir(dir);
	if(NULL==pdir){
		perror("opendir");
		return;
	}
	struct dirent *p;
	char path[100]={0};
	while((p=readdir(pdir))!=NULL){
		if(!strcmp(p->d_name,".")||!strcmp("..",p->d_name)) continue;
		memset(path,0,sizeof(path));
		sprintf(path,"%s%s%s",dir,"/",p->d_name);
		if(!strcmp(filename,p->d_name)&&p->d_type==8) printf("%s\n",path);
		if(p->d_type==4) searchFile(path,filename);
	}
	closedir(pdir);
	pdir=NULL;
}

int main(int argc,char* argv[]){
	judgeArgc(argc,3);
	searchFile(argv[1],argv[2]);
	return 0;
}
