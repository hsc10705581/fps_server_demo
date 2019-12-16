#include "database.h"
#include <iostream>

using namespace std;

class Auth
{
private:
    string search_user_sql;
    string insert_user_sql;
    Database * user_db;

public:
    Auth();
    ~Auth();
    bool regis(string username, string password);
    bool login(string username, string password);
};
