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
#include <unistd.h>

#include "sqlite3.h"
#include "db_driver.h"

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
        DB_ERROR("Can't open %s database:%s\n",abs_path_name,sqlite3_errmsg(db));
        return -1;
    }
    ret = sqlite3_close(db);
    if(SQLITE_OK != ret)
    {
        db_errno = sqlite3_errcode(db);
        DB_ERROR("Can't close %s database:%s\n",abs_path_name,sqlite3_errmsg(db));
        return -1;
    }

    if(exsit_flag)
        DB_DEBUG("%s database has been created.\n",abs_path_name);
    else
        DB_DEBUG("Create %s database OK.\n",abs_path_name);

    return 0;
}

int db_open(char *abs_path_name, sqlite3 **db)
{
    int ret = -1;

    ret = access(abs_path_name, F_OK);
    if(ret != 0)
    {
        DB_ERROR("%s database is not exsit!\n",abs_path_name);
        return -1;
    }

    if(*db)
    {
        DB_WARN("Warning : open db handle pointer is not NULL.\n");
    }

    ret = sqlite3_open(abs_path_name, db);
    if(SQLITE_OK != ret)
    {
        db_errno = sqlite3_errcode(*db);
        DB_ERROR("Can't open %s database:%s\n",abs_path_name,sqlite3_errmsg(*db));
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
        DB_ERROR("Can't close database:%s\n",sqlite3_errmsg(*db));
        return -1;
    }

    return 0;
}

