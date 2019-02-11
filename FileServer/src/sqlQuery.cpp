#include "funcpp.h"

//查询返回一个值,0成功,-1失败,buf为传出参数
int SqlSingleSelect(char* sql,char* buf)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    const char *server="localhost";
    const char *user="root";
    const char *password="xutianyu";
    const char *database="test";

    printf("%s\n",sql);

    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
        printf("Error connecting to database:%s\n",mysql_error(conn));
        return -1;
    }
    else{
        printf("Connected...\n");
    }
    t=mysql_query(conn,sql);
    if(t){
        printf("Error making query:%s\n",mysql_error(conn));
        mysql_close(conn);
        return -1;
    }
    else{
        res=mysql_use_result(conn);
        if(res){
            row=mysql_fetch_row(res);
            strcpy(buf,row[0]);
        }
        else{
            printf("Don't find data\n");
            mysql_free_result(res);
            mysql_close(conn);
            return -1;
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return 0;
}

