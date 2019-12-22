#include "controller.h"

Controller::Controller()
{
    auth = new Auth();
    sender = new Sender();
    sendloopCount = 0;
}

Controller::~Controller()
{
    delete auth;
    delete sender;
}

bool Controller::sendloop()
{
    //sleep(1);
    time_t timer = time(0);
    while (true) {
        time_t temp = time(0);
        if (difftime(temp, timer) > 0.03) {
            //printf("time: %s\n", ctime(&temp));
            break;
        }
    }
    sendloopCount++;
    //printf("sendloop count: %d\n", sendloopCount);
    json players = this->playerMap2Json();
    list<int>::iterator itor = clientList.begin();
    while (itor != clientList.end()) {
        //printf("send to clientfd: %d\n", *itor);
        itor++;
        this->sender->playerValueSender(*itor, players, sendloopCount);
    }
    return true;
}

void Controller::receiveMessage(int clientfd, json j)
{
    //printf("result: %s\n", j["test"].get<std::string>().c_str());
    std::string field;
    if (j.contains("field")) {
        field = j["field"].get<std::string>();
    }
    else {
        // does not contains field, show error
        exit(-1);
    }
    json content;
    if (j.contains("content")) {
        content = j["content"];
    }
    else {
        // does not contains content, show error
        exit(-1);
    }

    //printf("result: %s\n", content["password"].get<std::string>().c_str());
    if (field == "register") {
        this->registerHandler(clientfd, content);
    }
    else if (field == "login") {
        this->loginHandler(clientfd, content);
    }
    else if (field == "move") {
        this->moveHandler(clientfd, content);
    }
    else {
        // default
    }
}

void Controller::clientConnected(int clientfd)
{
    //printf("connected: %d\n", clientfd);
    if (clientfd == 4) {
        // 初始化的时候会connect一个fd=4？
        return;
    }
    this->clientList.push_back(clientfd);
}

void Controller::clientClosed(int clientfd)
{
    this->clientList.remove(clientfd);
}

json Controller::playerMap2Json()
{
    json ret;
    map<int, Player *>::iterator iter = playerMap.begin();
    while (iter != playerMap.end()) {
        ret.push_back(iter->second->toJson());
    }
    return ret;
}

// handlers
void Controller::registerHandler(int clientfd, json content)
{
    std::string username = content["username"].get<std::string>();
    std::string password = content["password"].get<std::string>();
    int status = auth->regis(username, password);
    this->sender->registerSender(clientfd, status);
}

void Controller::loginHandler(int clientfd, json content)
{
    std::string username = content["username"].get<std::string>();
    std::string password = content["password"].get<std::string>();
    int status = auth->login(username, password);
    this->sender->loginSender(clientfd, status);

    Player * player = new Player();
    playerMap.insert(map<int, Player*>::value_type(clientfd, player));
    moveCountMap.insert(map<int, unsigned int>::value_type(clientfd, 0));
}

void Controller::moveHandler(int clientfd, json content)
{
    unsigned int count = content["count"];
    map<int, unsigned int>::iterator it = moveCountMap.find(clientfd);
    if (it == moveCountMap.end()) {
        // no such clientfd
        return;
    }
    unsigned int cur_count = it->second;
    if (cur_count >= count) {
        // 服务器已经处理之后的请求，因此忽略该请求
        return;
    }
    // TODO:
    // 判断接收到的数据是否为float
    
    Vector3 position(content["position_x"], content["position_y"], content["position_z"]);
    Vector3 rotation(content["rotation_x"], content["rotation_y"], content["rotation_z"]);
    Player * player = playerMap.find(clientfd)->second;
    player->setPosition(position);
    player->setRotation(rotation);
}
