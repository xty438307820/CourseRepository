#include <func.h>

int main()
{
    unsigned int host=0x1234;
    printf("0x%x\n",host);
    unsigned int netw=htons(host);
    printf("0x%x\n",netw);

    struct in_addr sock;
    char ip[]="192.168.135.130";
    inet_aton(ip,&sock);
    printf("%x\n",sock.s_addr);
    char *p1;
    p1=inet_ntoa(sock);
    printf("%s\n",p1);
    return 0;
}
