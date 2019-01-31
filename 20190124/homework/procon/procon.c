#include<func.h>

int main(){
	int semid=semget(1000,2,0600|IPC_CREAT);
	if(-1==semid){
		perror("semget");
		return -1;
	}
	semctl(semid,0,SETVAL,5);
	semctl(semid,1,SETVAL,0);
	struct sembuf sopp,sopv;
	int i=5;
	if(!fork()){
		//消费者
		sopp.sem_num=1;
		sopp.sem_op=-1;
		sopp.sem_flg=SEM_UNDO;
		sopv.sem_num=0;
		sopv.sem_op=1;
		sopv.sem_flg=SEM_UNDO;
		while(i--){
			sleep(2);
			printf("Consumer:place=%d,product=%d\n",semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
			semop(semid,&sopp,1);
			printf("Consume\n");
			semop(semid,&sopv,1);
			printf("Consumer:place=%d,product=%d\n",semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
		}
	}
	else{
		sopp.sem_num=0;
		sopp.sem_op=-1;
		sopp.sem_flg=SEM_UNDO;
		sopv.sem_num=1;
		sopv.sem_op=1;
		sopv.sem_flg=SEM_UNDO;
		while(i--){
			sleep(1);
			printf("Producer:place=%d,product=%d\n",semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
			semop(semid,&sopp,1);
			printf("Produce\n");
			semop(semid,&sopv,1);
			printf("Producer:place=%d,product=%d\n",semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
		}
		wait(NULL);
		semctl(semid,0,IPC_RMID);
	}
	return 0;
}
