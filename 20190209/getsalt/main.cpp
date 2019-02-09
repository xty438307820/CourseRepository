#include "funcpp.h"

void get_salt(char* salt,char *passwd){
    int i,j;
    for(i=0,j=0;passwd[i]&&j!=3;++i){
        if(passwd[i]=='$') j++;
    }
    strncpy(salt,passwd,i-1);
}

int main()
{
    struct spwd *sp;
    char *passwd;
    char salt[512]={0};

    passwd=getpass("请输入密码:");
    if((sp=getspnam("test1"))==NULL){
        perror("getspnam");
        return -1;
    }
    cout<<sp->sp_pwdp<<endl;

    get_salt(salt,sp->sp_pwdp);
    cout<<"-----------------------------"<<endl;
    if(strcmp(sp->sp_pwdp,crypt(passwd,salt))==0) cout<<"ok"<<endl;
    else cout<<"not ok"<<endl;

    return 0;
}

