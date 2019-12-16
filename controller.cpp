#include "controller.h"

Controller::Controller()
{
    auth = new Auth();
}

Controller::~Controller()
{
    delete auth;
}

void Controller::receiveMessage(json j)
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
        int result = auth->regis(content["username"], content["password"]);
        switch(result) {
            case 0:
            {
                printf("register successfully.");
                break;
            }
            case -1:
            {
                break;
            }
        }
    }
    else if (field == "login") {
        int result = auth->login(content["username"], content["password"]);
        switch(result) {
            case 0:
            {
                printf("login successfully.");
                break;
            }
            case -1:
            {
                break;
            }
        }
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
