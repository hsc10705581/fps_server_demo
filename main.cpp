#include "epoll_server.h"

int main(int argc, char *argv[])
{
    EpollServer * server = new EpollServer();
    while(1)
    {
        server->mainloop();
    }
    close(listener); //关闭socket
    close(epfd);    //关闭内核   不在监控这些注册事件是否发生
    return 0;
}