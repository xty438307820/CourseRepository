#include <func.h>

int main()
{   
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex,NULL);
    int ret=pthread_mutex_unlock(&mutex);
    ret=pthread_mutex_unlock(&mutex);
    printf("%d\n",ret);
    return 0;
}

