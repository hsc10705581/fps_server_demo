#include "database.h"

Database::Database()
{
    mysql = mysql_init(NULL);

    if(!mysql) {
        std::cout<<"Error:"<<mysql_error(mysql);
        exit(1);
    }
}

Database::~Database()
{
    if(mysql) {
        mysql_close(mysql);
    }
}

bool Database::initDB(string host, string user,string pwd,string db_name, int port)
{
    mysql = mysql_real_connect(mysql, host.c_str(), user.c_str(), pwd.c_str(), db_name.c_str(), port, NULL, 0);
    if(!mysql) {
        cout << "Error: " << mysql_error(mysql);
        exit(1);
    }
    return true;
}

bool Database::exeSQL(std::string sql)
{
    //mysql_query()执行成功返回0,执行失败返回非0值。
    if (mysql_query(mysql,sql.c_str())) {
        cout<<"Query Error: "<<mysql_error(mysql);
        return false;
    }

    result = mysql_store_result(mysql);

    if (result) {
        //获取结果集中总共的字段数，即列数
        int               num_fields = mysql_num_fields(result);
        unsigned long long  num_rows = mysql_num_rows(result);

        for(unsigned long long i = 0; i < num_rows; i++) {
            row = mysql_fetch_row(result);
            if(!row) {
                break;
            }

            for(int j=0;j<num_fields;j++) {
                cout<<row[j]<<"\t\t";
            }
            cout<<endl;
        }
    } else {
        //代表执行的是update,insert,delete类的非查询语句
        if (mysql_field_count(mysql) == 0) {
            // 返回update,insert,delete影响的行数
            unsigned long long num_rows = mysql_affected_rows(mysql);

            return num_rows;
        } else {
            cout << "Get result error: " << mysql_error(mysql);
            return false;
        }
    }

    return true;
}
