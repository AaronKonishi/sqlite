#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "db_driver.h"

static void print_usage(const char *process)
{
    printf("Usage:\n");
    printf("%s version\n",process);
    printf("%s create db_name\n",process);
    printf("%s open db_name\n",process);
}

static void print_help(void)
{
    printf("1.create table\n");
    printf("2.create view\n");
    printf("3.create index\n");
    printf("4.alter table\n");
    printf("5.drop table\n");
    printf("0.to exit\n");

    printf("\n");
}

static void upper_to_lower(char *str)
{
    int i,len;
    len = strlen(str);
    for(i = 0; i < len; i++)
    {
        if(isalpha(str[i]) && isupper(str[i]))
            str[i] = tolower(str[i]);
    }
}

static void create_table(sqlite3 **db)
{
    int ret = -1;
    char name[256],table[1024];

    printf("Input table name:");
    fgets(name, 256, stdin);
    
    printf("Input table information:");
    fgets(table, 1024, stdin);

    // take off '\n'
    name[strlen(name)-1] = '\0';
    table[strlen(table)-1] = '\0';
    
    ret = db_table_create(db, name, table);
    if(ret)
    {
        // here tell you how to use errno and errmsg
        printf("create, %s\n", db_errmsg(*db,&ret));
        printf("ret = %d\n",ret);
    }
}

static void create_view(sqlite3 **db)
{
    int ret = -1;
    char name[256],table[1024];

    printf("Input view name:");
    fgets(name, 256, stdin);
    
    printf("Input sql statement:");
    fgets(table, 1024, stdin);

    // take off '\n'
    name[strlen(name)-1] = '\0';
    table[strlen(table)-1] = '\0';
    
    ret = db_view_create(db, name, table);
    if(ret)
    {
        // here tell you how to use errno and errmsg
        printf("create, %s\n", db_errmsg(*db,&ret));
        printf("ret = %d\n",ret);
    }
}

#define BUFFER_LEN  1024

static void main_loop(sqlite3 **db)
{
    char buffer[BUFFER_LEN];

    for(;;)
    {
        printf("\nInput h/help/.help command for help, quit/.quit/exit/.exit/0/q to exit:\n");
        printf("Your choice:");

        fgets(buffer, BUFFER_LEN, stdin);
        upper_to_lower(buffer);  // upper to lower

        // quit/exit judge
        if((strstr(buffer, "help") != NULL) || (buffer[0] == 'h'))
        {
            print_help();
            continue;
        }
        else if((strstr(buffer, "quit") != NULL) || (strstr(buffer, "exit") != NULL))
        {
            break;
        }
        else if((buffer[0] == '0') || (buffer[0] == 'q'))
        {
            break;
        }

        switch(buffer[0])
        {
            case '1':  // create table
                create_table(db);
                break;

            case '2':  // create view
                create_view(db);
                break;

            case '3':  // create index
                break;

            case '4':  // alter table
                break;

            case '5':  // drop table
                break;

            default:
                print_help();
                break;
        }
    }
}

int main(int argc, char *argv[])
{
    int ret = -1;
        
    if(argc < 2)
    {
        print_usage(argv[0]);
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
        
        main_loop(&db);
        
        printf("Close %s database.\n", argv[2]);
        db_close(&db);
    }
    else
    {
        print_usage(argv[0]);
    }
    
    return 0;
}


