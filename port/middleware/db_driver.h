//////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief the head file of database api-driver.
 *
 * @author Aaron Konishi, zhangqiyin@hangsheng.com.cn or zh5202@163.com
 *
 * @version HS-DB V1.0.0A
 *
 * @date 2017-04-13
 *
 * @copyright 2016 Shenzhen Hangsheng Electronics CO.,LTD.
 *
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef DB_DRIVER_H
#define DB_DRIVER_H
#include "sqlite3.h"

#define _DEBUG_

extern const char *db_version(void);
extern int db_create(char *abs_path_name);

extern int db_open(char *abs_path_name, sqlite3 **db);
extern int db_close(sqlite3 **db);

extern int db_table_create(sqlite3 **db, char *table_name, char *table_info);
extern int db_table_drop(sqlite3 **db, char *table_name);
extern int db_table_truncate(sqlite3 **db, char *table_name);
extern int db_table_alter_add(sqlite3 **db, char *table_name, char *name, char *type);
extern int db_table_alter_drop(sqlite3 **db, char *table_name, char *name);
extern int db_table_alter_change(sqlite3 **db, char *table_name, char *old_name, char *new_name, char *new_type);
extern int db_table_alter_modify(sqlite3 **db, char *table_name, char *name, char *new_type);
extern int db_table_alter_rename(sqlite3 **db, char *table_name, char *new_name);

extern int db_view_create(sqlite3 **db, char *view_name, char *view_sql);
extern int db_view_drop(sqlite3 **db, char *view_name);

extern const char *db_errmsg(sqlite3 *db, int *errno);
extern int db_sql_query_malloc(char **sql_ptr, char *sql_cmd, char *table_name, 
                               char *table_operate, int extra_len);
extern void db_sql_query_release(char **sql_ptr);

#endif

