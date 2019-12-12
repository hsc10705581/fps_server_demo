#include "controller.h"
#include "properties.h"

class EpollServer
{
private:
    int epfd;
    int listener;
    static struct epoll_event events[EPOLL_SIZE];

public:
    EpollServer();
    ~EpollServer();
    void mainloop();
};
