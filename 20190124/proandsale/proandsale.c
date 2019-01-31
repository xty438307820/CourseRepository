#include "func.h"

int main(){
	int semid=semget(1000,2,0600|IPC_CREAT);
	if(semid==-1){
		perror("semget");
		return -1;
	}
	semctl(semid,0,SETVAL,10);
	semctl(semid,1,SETVAL,0);
	int fk=fork();
	struct sembuf sopp,sopv;
	if(!fk){
		//消费者
		sopp.sem_num=1;
		sopp.sem_op=-1;
		sopp.sem_flg=SEM_UNDO;
		sopv.sem_num=0;
		sopv.sem_op=1;
		sopv.sem_flg=SEM_UNDO;
		int i=5;
		while(i--){
			sleep(2);
			printf("Consumer:place=%d,product=%d\n",semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
			semop(semid,&sopp,1);
			printf("Consumer done\n");
			semop(semid,&sopv,1);
			printf("Consumer:place=%d,product=%d\n",semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
		}
	}
	else{
		//生产者
		sopp.sem_num=0;
		sopp.sem_op=-1;
		sopp.sem_flg=SEM_UNDO;
		sopv.sem_num=1;
		sopv.sem_op=1;
		sopv.sem_flg=SEM_UNDO;
		int i=5;
		while(i--){
			sleep(1);
			printf("Producer:place=%d,product=%d\n",semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
			semop(semid,&sopp,1);
			printf("Producer done\n");
			semop(semid,&sopv,1);
			printf("Producer:place=%d,product=%d\n",semctl(semid,0,GETVAL),semctl(semid,1,GETVAL));
		}
		wait(NULL);
	}
	return 0;
}
