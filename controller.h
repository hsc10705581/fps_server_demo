//#include "common.h"
#include "player.h"
#include "region.h"
#include "auth.h"
#include "sender.h"

#include "nlohmann/json.hpp"
#include <map>
#include <list>
#include <unistd.h>
#include <ctime>
#include <time.h>

using namespace std;
using namespace nlohmann;

class Controller
{
private:
    Auth * auth;
    Sender * sender;
    list<int> clientList;
    int sendloopCount; // count 用于记录loop次序
    map<int, Player *> playerMap; // 一个clientfd对应一个Player
    map<int, unsigned int> moveCountMap; // 一个clientfd对应一个count整型，用于记录已处理的move请求的最大count
    map<int, unsigned int> jumpCountMap; // 一个clientfd对应一个count整型，用于记录已处理的jump请求的最大count
    map<int, unsigned int> shootCountMap; // 一个clientfd对应一个count整型，用于记录已处理的shoot请求的最大count
    void registerHandler(int clientfd, json content);
    void loginHandler(int clientfd, json content);
    void moveHandler(int clientfd, json content);
    void jumpHandler(int clientfd, json content);
    void shootHandler(int clientfd, json content);
    void hitHandler(int clientfd, json content);
    json playerMap2Json();
    
public:
    Controller();
    ~Controller();
    bool sendloop(sem_t mutex); // 向所有client发送数据
    void receiveMessage(int clientfd, json j);
    void clientConnected(int clientfd);
    void clientClosed(int clientfd);
};