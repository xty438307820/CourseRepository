#include "func.h"

void printdate(char* tim){
	int i;
	int flag=0;
	int len=strlen(tim);
	for(i=0;i<len;i++){
		if(tim[i]==' '){
			i++;break;
		}
	}
	while(1){
		if(tim[i]==':'){
			flag++;
			if(flag==2) break;
		}
		printf("%c",tim[i++]);
	}
}

void printmode(mode_t mode){
	int i,j,k;
	int high4=mode>>12;
	if(high4==8) printf("-");
	else if(high4==4) printf("d");
	else printf(" ");
	for(i=2;i>=0;i--){
		int inum=(mode>>3*i)%(1<<3);
		if(inum&1<<2) printf("r");
		else printf("-");
		if(inum&1<<1) printf("w");
		else printf("-");
		if(inum&1) printf("x");
		else printf("-");
	}
}

int main(int argc,char* argv[]){
	judgeArgc(argc,2);
	DIR *pdir=opendir(argv[1]);
	if(NULL==pdir){
		perror("opendir");
		return -1;
	}
	struct dirent *p;
	struct stat sta;
	char path[100]={0};
	char tim[30]={0};
	while((p=readdir(pdir))!=NULL){
		if(p->d_name[0]=='.'||!strcmp(".",p->d_name)||!strcmp("..",p->d_name)) continue;
		memset(path,0,sizeof(path));
		memset(tim,0,sizeof(tim));
		sprintf(path,"%s%s%s",argv[1],"/",p->d_name);
		if(-1==stat(path,&sta)){
			perror("stat");
			continue;
		}
		strcpy(tim,ctime(&sta.st_mtime));
		printmode(sta.st_mode);
		printf(" %ld %s %s %6ld ",sta.st_nlink,getpwuid(sta.st_uid)->pw_name,
				getgrgid(sta.st_gid)->gr_name,sta.st_size);
		printdate(tim);
		printf(" %s\n",p->d_name);
	}
	closedir(pdir);
	pdir=NULL;
	return 0;
}
