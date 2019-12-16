#include "auth.h"

Auth::Auth()
{
    user_db = new Database();
    user_db->initDB("127.0.0.1", "mangos", "mangos", "mangos", 3306);
    search_user_sql = "";
    insert_user_sql = "";
}

Auth::~Auth()
{

}

bool Auth::regis(string username, string password)
{

}

bool Auth::login(string username, string password)
{

}