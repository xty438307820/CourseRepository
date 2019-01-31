#include<func.h>

#define N 10000000

int main(){
	int shmid=shmget(1000,4096,0600|IPC_CREAT);
	if(-1==shmid){
		perror("shmget");
		return -1;
	}
	int *p=(int*)shmat(shmid,NULL,0);
	p[0]=0;
	int semid=semget(1000,1,0600|IPC_CREAT);
	if(-1==semid){
		perror("semget");
		return -1;
	}
	semctl(semid,0,SETVAL,1);
	struct sembuf sopp,sopv;
	sopp.sem_num=0;
	sopp.sem_op=-1;
	sopp.sem_flg=SEM_UNDO;
	sopv.sem_num=0;
	sopv.sem_op=1;
	sopv.sem_flg=SEM_UNDO;
	int i;
	if(!fork()){
		for(i=0;i<N;i++){
			semop(semid,&sopp,1);
			p[0]=p[0]+1;
			semop(semid,&sopv,1);
		}
	}
	else{
		for(i=0;i<N;i++){
			semop(semid,&sopp,1);
			p[0]=p[0]+1;
			semop(semid,&sopv,1);
		}
		wait(NULL);
		printf("%d\n",p[0]);
		shmctl(shmid,IPC_RMID,NULL);
	}
	return 0;
}
