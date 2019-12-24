#include "auth.h"

Auth::Auth()
{
    user_db = new Database();
    user_db->initDB("localhost", "mangos", "mangos", "logs", 3306);
    search_user_sql = "select password from fps_user where username=\"%s\"";
    insert_user_sql = "insert into fps_user (username, password) VALUES (\"%s\", \"%s\")";
    exist_sql = "select * from fps_user where username=\"%s\"";
}

Auth::~Auth()
{
    delete user_db;
}

int Auth::regis(string username, string password)
{
    if (this->isExist(username)) {
        // 用户已存在，无法注册
        return -1;
    }
    else {
        char sql[insert_user_sql.length()];
        sprintf(sql, insert_user_sql.c_str(), username.c_str(), password.c_str());
        MYSQL_RES* result = user_db->exeSQL(sql);
        if (result) {
            // error
            exit(1);
        }
        else {
            return 0;
        }
    }
}

int Auth::login(string username, string password)
{
    if (this->isExist(username)) {
        char sql[search_user_sql.length()];
        sprintf(sql, search_user_sql.c_str(), username.c_str());
        MYSQL_RES* result = user_db->exeSQL(sql);
        if (result) {
            //int num_fields = mysql_num_fields(result);
            MYSQL_ROW row = mysql_fetch_row(result);
            std::string row_pw(row[0]);
            if (!row) {
                // 用户不存在，无法登录
                return -1;
            }
            else if (password == row_pw) {
                // 登陆成功
                return 0;
            }
            else {
                // 密码错误
                return -2;
            }
        }
        else {
            // error
            exit(1);
        }
    }
    else {
        // 用户不存在，无法登录
        return -1;
    }
}

bool Auth::isExist(string username)
{
    char sql[exist_sql.length()];
    sprintf(sql, exist_sql.c_str(), username.c_str());
    MYSQL_RES* result = user_db->exeSQL(sql);
    if (result) {
        unsigned long long  num_rows = mysql_num_rows(result);
        if (num_rows == 0) {
            return false;
        }
        else {
            return true;
        }
    }
    else {
        // error
        exit(1);
    }

}