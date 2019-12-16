#include "controller.h"

Controller::Controller()
{

}

Controller::~Controller()
{

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

    }
    else if (field == "login") {

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
