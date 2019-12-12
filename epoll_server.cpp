#include "epoll_server.h"

EpollServer::EpollServer()
{
    //服务器IP + port
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    //创建监听socket
    int listener = socket(PF_INET, SOCK_STREAM, 0);
    if(listener < 0) { perror("listener"); exit(-1);}
    printf("listen socket created \n");
    //绑定地址
    if( bind(listener, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind error");
        exit(-1);
    }
    //监听
    int ret = listen(listener, 5);
    if(ret < 0) { perror("listen error"); exit(-1);}
    printf("Start to listen: %s:%d\n", SERVER_IP, SERVER_PORT);
    //在内核中创建事件表
    int epfd = epoll_create(EPOLL_SIZE);
    if(epfd < 0) { perror("epfd error"); exit(-1);}
    printf("epoll created, epollfd = %d\n", epfd);
    //static struct epoll_event events[EPOLL_SIZE];
    //往内核事件表里添加事件
    addfd(epfd, listener, true);
}

EpollServer::~EpollServer()
{
    
}



