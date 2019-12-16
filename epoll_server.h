#include "controller.h"
#include "properties.h"

#include <iostream>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
using namespace nlohmann;

class EpollServer
{
private:
    Controller * controller;
    int epfd;
    int listener;
    struct epoll_event events[EPOLL_SIZE];
    //struct epoll_event * events;
    list<int> clients_list;
    int receiveMessageHandler(int clientfd);

public:
    EpollServer();
    ~EpollServer();
    bool mainloop(); // false报错, true无错
    void addfd( int epollfd, int fd, bool enable_et );
    int getEpfd();
    int getListener();
};
