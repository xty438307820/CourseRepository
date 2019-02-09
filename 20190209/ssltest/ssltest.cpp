#include "funcpp.h"

int main()
{
    unsigned char buf[30]="helloworld";
    unsigned char *md;
    size_t i;
    md=MD5(buf,strlen((char*)buf),NULL);
    cout<<md<<endl;
    for(i=0;i<strlen((char*)md);i++){
        printf("%x",md[i]);
    }
    cout<<endl;

    return 0;
}

