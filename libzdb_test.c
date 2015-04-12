#include <zdb.h>
#include <stdio.h>
#include <stdlib.h>
#include "URL.h"
#include "Connection.h"
#include "ConnectionPool.h"
#include "Exception.h"
#include "SQLException.h"

int main(int argc, char **argv)
{
    URL_T url = URL_new("mysql://localhost/testdb?user=root&password=");
    ConnectionPool_T pool = ConnectionPool_new(url);
    ConnectionPool_start(pool);

    Connection_T con = ConnectionPool_getConnection(pool);
    ResultSet_T result = Connection_executeQuery(con, "select * from writers");
    while(ResultSet_next(result))
    {   
        const char *name = ResultSet_getStringByName(result, "name");
        printf("%s\n", name);
    }   

    Connection_close(con);
    ConnectionPool_free(&pool);
    URL_free(&url);
}
