#include <stdio.h>
#include <string.h>

#include "db_driver.h"

#define DB_PATH     "./testDB.db"

int main(int argc, char *argv[])
{
    int ret = -1;
    
    if(argc < 2)
    {
        printf("Usage:\n");
        printf("%s version\n",argv[0]);
        printf("%s create db_name\n",argv[0]);
        printf("%s open db_name\n",argv[0]);
        return -1;
    }
    
    if(strncmp(argv[1],"create",6) == 0)
    {
        if(argc < 3)
        {
            printf("%s create db_name\n",argv[0]);
            return -1;
        }
        db_create(argv[2]);
    }
    else if(strncmp(argv[1],"version",7) == 0)
    {
        printf("%s\n",db_version());
    }
    else if(strncmp(argv[1],"open",4) == 0)
    {
        if(argc < 3)
        {
            printf("%s open db_name\n",argv[0]);
            return -1;
        }
        
        sqlite3 *db = NULL;
        ret = db_open(argv[2],&db);
        if(0 != ret)
            return -1;

        printf("Open %s database OK.\n",argv[2]);
        //for(;;)
        {
            
        }

        db_close(&db);
    }
    
    return 0;
}


