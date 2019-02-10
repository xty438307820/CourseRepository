#include <funcpp.h>

//将from中间的空格分开,前部分为操作数,后部分为操作对象
void splitChar(char* from,char* operate,char* operand){
    int fromLen=strlen(from);
    int i=0,j=0;
    while(from[i]==' '&&i<fromLen) i++;
    while(from[i]!=' '&&i<fromLen) operate[j++]=from[i++];
    while(from[i]==' '&&i<fromLen) i++;
    j=0;
    while(operate[i]!=' '&&i<fromLen) operand[j++]=from[i++];
}

int main()
{
    char buf[128]=" cd /home/adam";
    char operate[32]={0};
    char operand[96]={0};
    splitChar(buf,operate,operand);
    cout<<buf<<endl;
    cout<<operate<<endl;
    cout<<operand<<endl;

    DIR *dir=opendir("~");
    if(NULL==dir){
        perror("opendir");
        return -1;
    }

    return 0;
}

