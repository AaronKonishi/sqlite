//////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief the source file of database api-driver.
 *
 * @author Aaron Konishi, zhangqiyin@hangsheng.com.cn or zh5202@163.com
 *
 * @version HS-DB V1.0.0a
 *
 * @date 2017-04-13
 *
 * @copyright 2016 Shenzhen Hangsheng Electronics CO.,LTD.
 *
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sqlite3.h"
#include "db_driver.h"

#ifdef _DEBUG_
#define DB_DEBUG(format, args...)     printf(format, ##args)
#else
#define DB_DEBUG(format, args...)
#endif
#define DB_WARN(format, args...)     printf(format, ##args)
#define DB_ERROR(format, args...)     printf(format, ##args)

#define DB_MIDDLEWARE_VERSION   "V1.0.0A"

#define DB_FULL_VERSION         "SQLITE Version : sqlite3-V" \
                                SQLITE_VERSION \
                                "\nSQLITE SourceID: " \
                                SQLITE_SOURCE_ID \
                                "\nMiddleware Version: " \
                                DB_MIDDLEWARE_VERSION

#define PACKAGE_SQL(ptr,format, args...) do{ \
        sprintf(ptr, format, ##args); \
        DB_DEBUG("%s\n",ptr); \
        }while(0);

static int db_errno = 0;

const char *db_version(void)
{
    return DB_FULL_VERSION;
}

int db_create(char *abs_path_name)
{
    int ret = -1;
    sqlite3 *db = NULL;
    char exsit_flag = 0;

    ret = access(abs_path_name, F_OK);
    if(ret == 0)
        exsit_flag = 1;
    
    ret = sqlite3_open(abs_path_name, &db);
    if(SQLITE_OK != ret)
    {
        db_errno = sqlite3_errcode(db);
        DB_ERROR("Error : Can't open %s database:%s\n",abs_path_name,sqlite3_errmsg(db));
        return -1;
    }
    ret = sqlite3_close(db);
    if(SQLITE_OK != ret)
    {
        db_errno = sqlite3_errcode(db);
        DB_ERROR("Error : Can't close %s database:%s\n",abs_path_name,sqlite3_errmsg(db));
        return -1;
    }

    if(exsit_flag)
        DB_DEBUG("Info : %s database has been created.\n",abs_path_name);
    else
        DB_DEBUG("Info : Create %s database OK.\n",abs_path_name);

    return 0;
}

int db_open(char *abs_path_name, sqlite3 **db)
{
    int ret = -1;
    
    ret = access(abs_path_name, F_OK);
    if(ret != 0)
    {
        DB_ERROR("Error : %s database is not exsit!\n",abs_path_name);
        return -1;
    }

    if(*db)
    {
        DB_WARN("Warning : open db handle pointer is not NULL.\n");
        *db = NULL;
    }

    ret = sqlite3_open(abs_path_name, db);
    if(SQLITE_OK != ret)
    {
        db_errno = sqlite3_errcode(*db);
        DB_ERROR("Error : Can't open %s database:%s\n",abs_path_name,sqlite3_errmsg(*db));
        return -1;
    }
    
    return 0;
}

int db_close(sqlite3 **db)
{
    int ret = -1;

    if(*db == NULL)
    {
        DB_WARN("Warning : close db handle pointer is NULL.\n");
        return 0;
    }
    
    ret = sqlite3_close(*db);
    if(SQLITE_OK != ret)
    {
        db_errno = sqlite3_errcode(*db);
        DB_ERROR("Error : Can't close database:%s\n",sqlite3_errmsg(*db));
        return -1;
    }
    *db = NULL;

    return 0;
}

/*******************************************************************************
 * table_name : the name of the table which you want to be create.
 * table_info : the table information(row and column), the format is :
 *            "(data1_name data1_type,data2_name data2_type, ...)"
********************************************************************************/
int db_table_create(sqlite3 **db, char *table_name, char *table_info)
{
    int ret = -1;
    char *sql_state = NULL;
    char sql_cmd[] = "CREATE TABLE";

    if(NULL == *db)
    {
        DB_ERROR("Error : CREATE TABLE failed db_ptr is NULL!\n");
        return -1;
    }
    
    ret = db_sql_query_malloc(&sql_state, sql_cmd, table_name, table_info, 0);
    if(ret)
    {
        DB_ERROR("Error : CREATE TABLE failed!\n");
        return -1;
    }

    PACKAGE_SQL(sql_state, "%s %s %s;", sql_cmd, table_name, table_info);
    
    // 1.on create , no need callback;
    // 2.you can use zErrMsg, and use sqlite3_free() to free it;
    //   and you can use sqlite3_errmsg() to get error;
    ret = sqlite3_exec(*db, sql_state, NULL, NULL, NULL);
    if(SQLITE_OK != ret)
    {
        db_errno = sqlite3_errcode(*db);
        DB_ERROR("Error : CREATE TABLE failed, %s\n", sqlite3_errmsg(*db));
        db_sql_query_release(&sql_state);
        return -1;
    }
    
    db_sql_query_release(&sql_state);
    
    return 0;
}

int db_view_create(sqlite3 **db, char *view_name, char *view_sql)
{
    int ret = -1;
    char *sql_state = NULL;
    char sql_cmd[] = "CREATE VIEW";

    if(NULL == *db)
    {
        DB_ERROR("Error : CREATE VIEW failed db_ptr is NULL!\n");
        return -1;
    }
    
    ret = db_sql_query_malloc(&sql_state, sql_cmd, view_name, view_sql, 0);
    if(ret)
    {
        DB_ERROR("Error : CREATE VIEW failed!\n");
        return -1;
    }

    PACKAGE_SQL(sql_state, "%s %s AS %s;", sql_cmd, view_name, view_sql);
    
    // 1.on create , no need callback;
    // 2.you can use zErrMsg, and use sqlite3_free() to free it;
    //   and you can use sqlite3_errmsg() to get error;
    ret = sqlite3_exec(*db, sql_state, NULL, NULL, NULL);
    if(SQLITE_OK != ret)
    {
        db_errno = sqlite3_errcode(*db);
        DB_ERROR("Error : CREATE VIEW failed, %s\n", sqlite3_errmsg(*db));
        db_sql_query_release(&sql_state);
        return -1;
    }
    
    db_sql_query_release(&sql_state);
    
    return 0;
}

/*******************************************************************************
 * errno : return error number;
 * return error context or Warning.
********************************************************************************/
const char *db_errmsg(sqlite3 *db, int *errno)
{
    if(errno)
        *errno = db_errno;
    
    if(db)
        return sqlite3_errmsg(db);
    
    return "Warning: db is NULL, can't get errmsg!\n";
}

/*******************************************************************************
 * package sql-statement using malloc.
********************************************************************************/
int db_sql_query_malloc(char **sql_ptr, char *sql_cmd, char *table_name, char *table_operate, int extra_len)
{
    int sql_len = 0;

    /* sql_cmd + spacing + table_name + spacing + table_operate + ';' + '\0' + extra_len */
    sql_len = strlen(sql_cmd) + 1 + strlen(table_name) + 1 + strlen(table_operate) + 1 + 1 + extra_len;
    DB_DEBUG("Info : SQL-Query %d, ",sql_len);
    *sql_ptr = (char *)malloc(sql_len);
    if(NULL == *sql_ptr)
    {
        DB_WARN("Warning : malloc sql-query string failed!\n");
        return -1;
    }
    
    return 0;
}

void db_sql_query_release(char **sql_ptr)
{
    if(*sql_ptr)
        free(*sql_ptr);
}



