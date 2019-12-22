#include "sender.h"

Sender::Sender()
{
    
}

Sender::~Sender()
{

}

void Sender::jsonSender(int clientfd, json j)
{
    string result = j.dump();
    char message[BUF_SIZE];
    bzero(message, BUF_SIZE);
    //sprintf(message, result.c_str());
    //printf("register feedback %s.\n", result.c_str());
    if( send(clientfd, result.c_str(), BUF_SIZE, 0) < 0 ) { 
        //perror("error"); 
        //exit(-1);
    }
}

void Sender::registerSender(int clientfd, int status)
{
    json main;
    main["field"] = "register_feedback";
    json content;
    content["status"] = status;
    switch (status)
    {
    case 0:
        content["message"] = "register successfully!";
        break;
    case -1:
        content["message"] = "user already exists.";
        break;
    }
    main["content"] = content;
    this->jsonSender(clientfd, main);
}

void Sender::loginSender(int clientfd, int status)
{
    json main;
    main["field"] = "login_feedback";
    json content;
    content["status"] = status;
    switch (status)
    {
    case 0:
        content["message"] = "login successfully!";
        break;
    case -1:
        content["message"] = "no such user.";
        break;
    case -2:
        content["message"] = "password incorrect.";
        break;
    }
    main["content"] = content;
    this->jsonSender(clientfd, main);
}

void Sender::playerValueSender(int clientfd, json players, int count)
{
    json main;
    main["field"] = "player_values";
    json content;
    content["count"] = count;
    content["players"] = players;
    main["content"] = content;
    printf("send: %s\n", main.dump().c_str());
    this->jsonSender(clientfd, main);
    //printf("player value sending finished\n");
}
