#include <func.h>

int main()
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    const char *server="localhost";
    const char *user="root";
    const char *password="xutianyu";
    const char *database="test";

    char query[100]="select sname from student where sid=1";
    printf("%s\n",query);

    int t;
    unsigned int r;
    char buf[128]={0};
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
        res=mysql_use_result(conn);
        if(res){
            row=mysql_fetch_row(res);
            strcpy(buf,row[0]);
            printf("%s\n",buf);
            if(strcmp(buf,"xiongda")==0) printf("yes\n");
        }
        else{
            printf("Don't find data\n");
        }
        mysql_free_result(res);
    }

    mysql_close(conn);
    return 0;
}

