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
    printf("2.drop table\n");
    printf("3.truncate table\n");
    printf("4.alter table\n");
    printf("5.create view\n");
    printf("6.drop view\n");
    
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

static void drop_table(sqlite3 **db)
{
    int ret = -1;
    char name[256];

    printf("Input table name:");
    fgets(name, 256, stdin);
    
    // take off '\n'
    name[strlen(name)-1] = '\0';
    
    ret = db_table_drop(db, name);
    if(ret)
    {
        // here tell you how to use errno and errmsg
        printf("drop, %s\n", db_errmsg(*db,&ret));
        printf("error = %d\n",ret);
    }
}

#ifdef _SUPPORT
static void truncate_table(sqlite3 **db)
{
    int ret = -1;
    char name[256];

    printf("Input table name:");
    fgets(name, 256, stdin);
    
    // take off '\n'
    name[strlen(name)-1] = '\0';
    
    ret = db_table_truncate(db, name);
    if(ret)
    {
        // here tell you how to use errno and errmsg
        printf("truncate, %s\n", db_errmsg(*db,&ret));
        printf("error = %d\n",ret);
    }
}
#endif

static void alter_table(sqlite3 **db)
{
    char ch;
    int ret = -1;
    int choice = -1;
    char table_name[256],column[256],type[256];

    printf("You can do the following things:\n");
    printf("1.add column\n");
    printf("2.del column\n");
    printf("3.change column\n");
    printf("4.modify column\n");
    printf("5.rename table\n");
    printf("You choice:");

    scanf("%d",&choice);
    while((ch=getchar()) != '\n' && ch != EOF);

    printf("Input table name:");
    fgets(table_name, 256, stdin);
    table_name[strlen(table_name)-1] = '\0';
    
    switch(choice)
    {
        case 1:
            printf("Input column name:");
            fgets(column, 256, stdin);
            printf("Input column type:");
            fgets(type, 256, stdin);
            column[strlen(column)-1] = '\0';
            type[strlen(type)-1] = '\0';
            ret = db_table_alter_add(db, table_name, column, type);
            if(ret)
            {
                printf("add column, %s\n", db_errmsg(*db,&ret));
                printf("error = %d\n",ret);
            }
            break;
        case 2:
            #ifdef _SUPPORT
            printf("Input column name you want to delete:");
            fgets(column, 256, stdin);
            column[strlen(column)-1] = '\0';
            ret = db_table_alter_drop(db, table_name, column);
            if(ret)
            {
                printf("del column, %s\n", db_errmsg(*db,&ret));
                printf("error = %d\n",ret);
            }
            #else
            printf("SQLITE3 not support yet.\n");
            #endif
            break;
        case 3:
        {
            #ifdef _SUPPORT
            char old_column[256];
            printf("Input column old name:");
            fgets(old_column, 256, stdin);
            printf("Input column name:");
            fgets(column, 256, stdin);
            printf("Input column type:");
            fgets(type, 256, stdin);
            old_column[strlen(old_column)-1] = '\0';
            column[strlen(column)-1] = '\0';
            type[strlen(type)-1] = '\0';
            ret = db_table_alter_change(db, table_name, old_column, column, type);
            if(ret)
            {
                printf("change column, %s\n", db_errmsg(*db,&ret));
                printf("error = %d\n",ret);
            }
            #else
            printf("SQLITE3 not support yet.\n");
            #endif
            break;
        }
        case 4:
            #ifdef _SUPPORT
            printf("Input column name:");
            fgets(column, 256, stdin);
            printf("Input column new type:");
            fgets(type, 256, stdin);
            column[strlen(column)-1] = '\0';
            type[strlen(type)-1] = '\0';
            ret = db_table_alter_modify(db, table_name, column, type);
            if(ret)
            {
                printf("add column, %s\n", db_errmsg(*db,&ret));
                printf("error = %d\n",ret);
            }
            #else
            printf("SQLITE3 not support yet.\n");
            #endif
            break;
        case 5:
            printf("Input table new name:");
            fgets(column, 256, stdin);
            column[strlen(column)-1] = '\0';
            ret = db_table_alter_rename(db, table_name, column);
            if(ret)
            {
                printf("rename table, %s\n", db_errmsg(*db,&ret));
                printf("error = %d\n",ret);
            }
            break;
        default:
            printf("Your input is illegality!\n");
            break;
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
        printf("error = %d\n",ret);
    }
}

static void drop_view(sqlite3 **db)
{
    int ret = -1;
    char name[256];

    printf("Input view name:");
    fgets(name, 256, stdin);
    
    // take off '\n'
    name[strlen(name)-1] = '\0';
    
    ret = db_view_drop(db, name);
    if(ret)
    {
        // here tell you how to use errno and errmsg
        printf("drop, %s\n", db_errmsg(*db,&ret));
        printf("error = %d\n",ret);
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

            case '2':  // drop table
                drop_table(db);
                break;

            case '3':  // truncate table
                #ifdef _SUPPORT
                truncate_table(db); // not supported
                #else
                printf("SQLITE3 not support yet.\n");
                #endif
                break;

            case '4':  // alter table
                alter_table(db);
                break;

            case '5':  // create view
                create_view(db);
                break;

            case '6':  // drop view
                drop_view(db);
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


