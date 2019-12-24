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

bool Controller::sendloop(sem_t mutex)
{
    //sleep(1);
    time_t timer = time(0);
    while (true) {
        time_t temp = time(0);
        if (difftime(temp, timer) > 0.3) {
            //printf("time: %s\n", ctime(&temp));
            break;
        }
    }
    sendloopCount++;
    //printf("sendloop count: %d\n", sendloopCount);
    sem_wait(&mutex);
    json players = this->playerMap2Json();
    list<int>::iterator itor = clientList.begin();
    while (itor != clientList.end()) {
        //printf("send player values message to clientfd = %d\n", *itor);
        this->sender->playerValueSender(*itor, players, sendloopCount);
        
        itor++;
    }
    sem_post(&mutex);
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
    else if (field == "jump") {
        this->jumpHandler(clientfd, content);
    }
    else if (field == "shoot") {
        this->shootHandler(clientfd, content);
    }
    else if (field == "hit") {
        this->hitHandler(clientfd, content);
    }
    else {
        // default
    }
}

void Controller::clientConnected(int clientfd)
{
    printf("controller saves clientfd: %d\n", clientfd);
    if (clientfd == 4) {
        // 初始化的时候会connect一个fd=4？
        return;
    }
    this->clientList.push_back(clientfd);
}

void Controller::clientClosed(int clientfd)
{
    this->clientList.remove(clientfd);
    this->playerMap.erase(clientfd);
    this->moveCountMap.erase(clientfd);
    this->jumpCountMap.erase(clientfd);
    this->shootCountMap.erase(clientfd);
}

json Controller::playerMap2Json()
{
    json ret;
    map<int, Player *>::iterator iter = playerMap.begin();
    while (iter != playerMap.end()) {
        ret.push_back(iter->second->toJson(iter->first));
        iter++;
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
    if (this->playerMap.find(clientfd) != this->playerMap.end()) {
        this->sender->loginSender(clientfd, 0);
        return;
    }
    std::string username = content["username"].get<std::string>();
    std::string password = content["password"].get<std::string>();
    int status = auth->login(username, password);
    this->sender->loginSender(clientfd, status);

    Player * player = new Player();
    playerMap.insert(map<int, Player*>::value_type(clientfd, player));
    moveCountMap.insert(map<int, unsigned int>::value_type(clientfd, 0));
    jumpCountMap.insert(map<int, unsigned int>::value_type(clientfd, 0));
    shootCountMap.insert(map<int, unsigned int>::value_type(clientfd, 0));
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

    // 更新count值
    it->second = count;
    
    Vector3 position(content["position_x"], content["position_y"], content["position_z"]);
    Vector3 rotation(content["rotation_x"], content["rotation_y"], content["rotation_z"]);
    Player * player = playerMap.find(clientfd)->second;
    player->setPosition(position);
    player->setRotation(rotation);
}

void Controller::jumpHandler(int clientfd, json content)
{
    unsigned int count = content["count"];
    map<int, unsigned int>::iterator it = jumpCountMap.find(clientfd);
    if (it == jumpCountMap.end()) {
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

    // 更新count值
    it->second = count;

    // 广播动作数据 暂时用clientfd作为player id
    this->sender->playerActionSender(clientfd, "jump", clientfd);
}

void Controller::shootHandler(int clientfd, json content)
{
    unsigned int count = content["count"];
    map<int, unsigned int>::iterator it = shootCountMap.find(clientfd);
    if (it == shootCountMap.end()) {
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

    // 更新count值
    it->second = count;
    
    // 广播动作数据 暂时用clientfd作为player id
    this->sender->playerActionSender(clientfd, "shoot", clientfd);// 广播动作数据
}

void Controller::hitHandler(int clientfd, json content)
{
    int hit_fd = content["player_id"];
    map<int, Player *>::iterator iter = playerMap.begin();
    while (iter != playerMap.end()) {
        if (iter->first == hit_fd) {
            Player * player = iter->second;
            player->setHp(player->getHp() - 25.0f);
        }
        iter++;
    }
}
