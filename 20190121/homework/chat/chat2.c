#include "func.h"

int main(int argc,char* argv[]){
	judgeArgc(argc,3);
	int fdw=open(argv[1],O_WRONLY);
	if(-1==fdw){
		perror("open1");
		return -1;
	}
	int fdr=open(argv[2],O_RDONLY);
	if(-1==fdr){
		perror("open2");
		return -1;
	}
	printf("connection has established!\n");
	fd_set set;
	char ch[128]={0};
	int ret;
	while(1){
		FD_ZERO(&set);
		FD_SET(STDIN_FILENO,&set);
		FD_SET(fdr,&set);
		if(select(fdr+1,&set,NULL,NULL,NULL)>0){
			if(FD_ISSET(STDIN_FILENO,&set)){
				memset(ch,0,sizeof(ch));
				ret=read(STDIN_FILENO,ch,sizeof(ch));
				if(0==ret){
					printf("exit the app!\n");
					break;
				}
				write(fdw,ch,strlen(ch));
			}
			if(FD_ISSET(fdr,&set)){
				memset(ch,0,sizeof(ch));
				ret=read(fdr,ch,sizeof(ch));
				if(ret==0){
					printf("your partner has closed the app!\n");
					break;
				}
				printf("%s",ch);
			}
		}
	}
	close(fdr);
	close(fdw);
	return 0;
}
