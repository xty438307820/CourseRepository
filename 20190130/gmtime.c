#include <func.h>

void* fun(void *p){
    time_t tim=time(NULL);
    struct tm gt;
    gmtime_r(&tim,&gt);
    printf("%d %d %d %d:%d:%d\n",gt.tm_year+1900,gt.tm_mon+1,gt.tm_mday,gt.tm_hour,gt.tm_min,gt.tm_sec);
    sleep(5);
    tim=time(NULL);
    gmtime_r(&tim,&gt);
    printf("%d %d %d %d:%d:%d\n",gt.tm_year+1900,gt.tm_mon+1,gt.tm_mday,gt.tm_hour,gt.tm_min,gt.tm_sec);
}

int main()
{
    pthread_t threadid;
    pthread_create(&threadid,NULL,fun,NULL);
    sleep(2);
    time_t tim=time(NULL);
    printf("%s",ctime(&tim));
    struct tm gt;
    gmtime_r(&tim,&gt);
    printf("%d %d %d %d:%d:%d\n",gt.tm_year+1900,gt.tm_mon+1,gt.tm_mday,gt.tm_hour,gt.tm_min,gt.tm_sec);
    pthread_join(threadid,NULL);
    return 0;
}

