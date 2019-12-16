#include "player.h"
#include "region.h"
#include "auth.h"

#include "nlohmann/json.hpp"
#include <map>
#include <list>

using namespace std;
using namespace nlohmann;

class Controller
{
private:
    Auth * auth;
    list<int> clientList;
    map<int, Player *> clientPlayerMap; // 一个clientfd对应一个Player
    
public:
    Controller();
    ~Controller();
    void receiveMessage(json j);
    void clientConnected(int clientfd);
    void clientClosed(int clientfd);
};