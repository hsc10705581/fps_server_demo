#include <iostream>
#include <mysql/mysql.h>

using namespace std;

class Database
{
private:
     /**
     * 连接mysql句柄指针
     */
    MYSQL *mysql;

    /**
     * 指向查询结果的指针
     */
    MYSQL_RES *result;

    /**
     * 按行返回的查询信息
     */
    MYSQL_ROW row;

public:
    Database();
    ~Database();

    /**
     * 连接mysql
     *
     * @param host
     * @param user
     * @param pwd
     * @param db_name
     * @param port
     * @return
     */
    bool initDB(string host, string user,string pwd,string db_name, int port);

    /**
     * 执行sql语句
     *
     * @param sql
     * @return
     */
    bool exeSQL(string sql);
};