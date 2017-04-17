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

#ifdef _DEBUG_
#define DB_DEBUG(format, args...)     printf(format, ##args)
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

extern const char *db_version(void);
extern int db_create(char *abs_path_name);
extern int db_close(sqlite3 **db);
extern int db_open(char *abs_path_name, sqlite3 **db);


#endif

