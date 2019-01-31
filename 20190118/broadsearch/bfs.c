#include "func.h"

typedef struct{
	char path[100];
	char pre[25];
	int level;
}node,*pnode;

void bfs(char* ch,char* pre){
	node queue[100];
	int head=0;
	int tail=0;
	DIR *pdir;
	struct dirent *dr;
	char path[100];
	node p;
	strcpy(p.path,ch);
	strcpy(p.pre,ch);
	p.level=1;
	queue[tail++]=p;
	while(head!=tail){
		node cur=queue[head++];
		pdir=opendir(cur.path);
		if(NULL==pdir){
			perror("opendir");
			continue;
		}

		while((dr=readdir(pdir))!=NULL){
			if(!strcmp(dr->d_name,".")||!strcmp(dr->d_name,"..")) continue;
			if(dr->d_type==4){
				node t;
				memset(path,0,sizeof(path));
				sprintf(path,"%s%s%s",cur.path,"/",dr->d_name);
				strcpy(t.path,path);
				strcpy(t.pre,dr->d_name);
				t.level=cur.level+1;
				queue[tail++]=t;
				printf("%*s%s/%s\n",3*cur.level,"",cur.pre,dr->d_name);
			}
			else{
				printf("%*s%s/%s\n",3*cur.level,"",cur.pre,dr->d_name);
			}
		}

	}
}

int main(int argc,char* argv[]){
	judgeArgc(argc,2);
	printf("%s\n",argv[1]);
	bfs(argv[1],NULL);
	return 0;
}
