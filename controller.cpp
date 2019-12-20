#include "controller.h"

Controller::Controller()
{
    auth = new Auth();
    sender = new Sender();
}

Controller::~Controller()
{
    delete auth;
    delete sender;
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
        exit(0);
    }
    json content;
    if (j.contains("content")) {
        content = j["content"];
    }
    else {
        // does not contains content, show error
        exit(0);
    }

    //printf("result: %s\n", content["password"].get<std::string>().c_str());
    if (field == "register") {
        std::string username = content["username"].get<std::string>();
        std::string password = content["password"].get<std::string>();
        int status = auth->regis(username, password);
        this->sender->registerSender(clientfd, status);
    }
    else if (field == "login") {
        std::string username = content["username"].get<std::string>();
        std::string password = content["password"].get<std::string>();
        int status = auth->login(username, password);
        this->sender->loginSender(clientfd, status);
    }
    else {
        // default
    }
}

void Controller::clientConnected(int clientfd)
{

}

void Controller::clientClosed(int clientfd)
{

}
