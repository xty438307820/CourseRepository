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

    char query[100]="select * from student";
    printf("%s\n",query);

    int t;
    unsigned int r;
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
            while((row=mysql_fetch_row(res))!=NULL){
                for(r=0;r<mysql_num_fields(res);r++){
                    printf("%8s ",row[r]);
                }
                printf("\n");
            }
        }
        else{
            printf("Don't find data\n");
        }
        mysql_free_result(res);
    }

    mysql_close(conn);
    return 0;
}

