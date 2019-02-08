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

    char query[100]="insert into student(sname,sage,score) values('zhangsan',30,60)";
    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
        printf("Error connecting to database:%s\n",mysql_error(conn));
        return -1;
    }
    else{
        printf("Connected...\n");
    }
    t=mysql_query(conn,query);
    if(t){
        printf("Error making query:%s\n",mysql_error(conn));
    }
    else{
        printf("insert success\n");
    }
    
    mysql_close(conn);
    return 0;
}

