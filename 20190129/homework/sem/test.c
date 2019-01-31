#include <func.h>

int main()
{
    sem_t sem;
    sem_init(&sem,0,10);
    sem_wait(&sem);
    int ret;
    sem_getvalue(&sem,&ret);
    printf("%d\n",ret);
    return 0;
}

