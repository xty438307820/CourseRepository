#include <func.h>

int main()
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    const char* server="localhost";
    const char* user="root";
    const char* password="xutianyu";
    const char* database="test";

    char query[100]="update student set score=90 where sname='zhangsan'";
    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
        printf("Error connecting to database:%s\n",mysql_error(conn));
    }
    else{
        printf("Connecting...\n");
    }
    t=mysql_query(conn,query);
    if(t){
        printf("Error making query:%s\n",mysql_error(conn));
    }
    else{
        printf("update success\n");
    }

    mysql_close(conn);
    return 0;
}

