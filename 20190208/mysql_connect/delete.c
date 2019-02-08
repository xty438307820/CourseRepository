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

    char query[200]="delete from student where sname='zhangsan'";
    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
        printf("Error connecting to database:%s\n",mysql_error(conn));
    }
    else{
        printf("Connected...\n");
    }
    t=mysql_query(conn,query);
    if(t){
        printf("Error making query:%s\n",mysql_error(conn));
    }
    else{
        printf("delete success,delete row=%lu\n",(unsigned long)mysql_affected_rows(conn));
    }

    mysql_close(conn);
    return 0;
}

