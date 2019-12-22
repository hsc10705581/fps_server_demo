#include "properties.h"

#include <iostream>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#include "nlohmann/json.hpp"

using namespace std;
using namespace nlohmann;

class Sender
{
private:
    void jsonSender(int clientfd, json j);

public:
    Sender();
    ~Sender();
    void registerSender(int clientfd, int status);
    void loginSender(int clientfd, int status);
    void playerValueSender(int clientfd, json players, int count);
};
