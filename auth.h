#include "database.h"
#include <iostream>

using namespace std;

class Auth
{
private:
    string search_user_sql;
    string insert_user_sql;
    string exist_sql;
    Database * user_db;
    bool isExist(string username);

public:
    Auth();
    ~Auth();
    int regis(string username, string password);
    int login(string username, string password);
};
