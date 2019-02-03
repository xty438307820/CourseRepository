#include <func.h>

int main(int argc,char* argv[])
{
    judgeArgc(argc,2);
    struct hostent* phost=gethostbyname(argv[1]);
    printf("%s\n",phost->h_name);
    int i;
    for(i=0;phost->h_aliases[i]!=NULL;i++){
        printf("%s\n",phost->h_aliases[i]);
    }
    printf("%d\n",phost->h_addrtype);
    printf("%d\n",phost->h_length);
    char dit[20]={0};
    for(i=0;phost->h_addr_list[i]!=NULL;i++){
        memset(dit,0,sizeof(dit));
        printf("%s\n",inet_ntop(phost->h_addrtype,phost->h_addr_list[i],dit,sizeof(dit)));
    }
    return 0;
}
