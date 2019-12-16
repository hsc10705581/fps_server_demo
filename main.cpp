#include "epoll_server.h"

int main(int argc, char *argv[])
{
    EpollServer * server = new EpollServer();
    while(1)
    {
        if (!server->mainloop())
            break;
    }
    close(server->getListener()); //关闭socket
    close(server->getEpfd());    //关闭内核   不在监控这些注册事件是否发生
    return 0;
}