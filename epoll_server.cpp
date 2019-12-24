#include "epoll_server.h"

EpollServer::EpollServer()
{
    controller = new Controller();
    //events = new struct epoll_event[EPOLL_SIZE];

    //服务器IP + port
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    //创建监听socket
    this->listener = socket(PF_INET, SOCK_STREAM, 0);
    if(this->listener < 0) { perror("listener"); exit(-1);}
    printf("listen socket created \n");
    //绑定地址
    if( bind(this->listener, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind error");
        exit(-1);
    }
    //监听
    int ret = listen(this->listener, 5);
    if(ret < 0) { perror("listen error"); exit(-1);}
    printf("Start to listen: %s:%d\n", SERVER_IP, SERVER_PORT);
    //在内核中创建事件表
    this->epfd = epoll_create(EPOLL_SIZE);
    if(this->epfd < 0) { perror("epfd error"); exit(-1);}
    printf("epoll created, epollfd = %d\n", this->epfd);
    //static struct epoll_event events[EPOLL_SIZE];
    //往内核事件表里添加事件
    addfd(this->epfd, this->listener, true);
}

EpollServer::~EpollServer()
{
    delete controller;
}

bool EpollServer::mainloop(sem_t mutex)
{
    //epoll_events_count表示就绪事件的数目
    int epoll_events_count = epoll_wait(this->epfd, this->events, EPOLL_SIZE, -1);
    if(epoll_events_count < 0) {
        perror("epoll failure");
        return false;
    }

    //printf("epoll_events_count = %d\n", epoll_events_count);
    //处理这epoll_events_count个就绪事件
    for(int i = 0; i < epoll_events_count; ++i)
    {
        int sockfd = this->events[i].data.fd;
        //新用户连接
        if(sockfd == listener)
        {
            struct sockaddr_in client_address;
            socklen_t client_addrLength = sizeof(struct sockaddr_in);
            int clientfd = accept( this->listener, ( struct sockaddr* )&client_address, &client_addrLength );

            printf("client connection from: %s : % d(IP : port), clientfd = %d \n",
            inet_ntoa(client_address.sin_addr),
            ntohs(client_address.sin_port),
            clientfd);

            addfd(this->epfd, clientfd, true);////把这个新的客户端添加到内核事件列表

            // 服务端用list保存用户连接
            clients_list.push_back(clientfd);
            printf("Add new clientfd = %d to epoll\n", clientfd);
            printf("Now there are %d clients int the chat room\n", (int)clients_list.size());

            // 服务端发送欢迎信息
            // printf("welcome message\n");
            char message[BUF_SIZE];
            bzero(message, BUF_SIZE);
            sprintf(message, SERVER_WELCOME, clientfd);
            //printf("send message: %s\n", message);
            //int ret = send(clientfd, message, BUF_SIZE, 0);
            printf("send welcome message to clientfd = %d\n", clientfd);
            send(clientfd, message, BUF_SIZE, 0);
            //if(ret < 0) { perror("send error"); exit(-1); }
        }
        //客户端唤醒//处理用户发来的消息，并广播，使其他用户收到信息
        else
        {
            //printf("receiver wait lock\n");
            sem_wait(&mutex);
            int ret = this->receiveMessageHandler(sockfd);
            sem_post(&mutex);
            //printf("receiver release lock\n");
            if(ret < 0) { 
                perror("error");
                this->controller->clientClosed(sockfd);
                close(sockfd);
                clients_list.remove(sockfd); //server remove the client
                //exit(-1); 
            }
        }
    }
    return true;
}

bool EpollServer::sendloop(sem_t mutex)
{
    return this->controller->sendloop(mutex);
}

/**
  * @param epollfd: epoll handle
  * @param fd: socket descriptor
  * @param enable_et : enable_et = true, epoll use ET; otherwise LT
**/
void EpollServer::addfd( int epollfd, int fd, bool enable_et )
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if( enable_et )
        ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0)| O_NONBLOCK);
    printf("fd added to epoll!\n\n");
    
    this->controller->clientConnected(fd);
}

int EpollServer::getEpfd()
{
    return this->epfd;
}

int EpollServer::getListener()
{
    return this->listener;
}

int EpollServer::receiveMessageHandler(int clientfd)
{
    // buf[BUF_SIZE] receive new chat message
    // message[BUF_SIZE] save format message
    char buf[BUF_SIZE], message[BUF_SIZE];
    bzero(buf, BUF_SIZE);
    bzero(message, BUF_SIZE);

    // receive message
    //printf("read from client(clientID = %d)\n", clientfd);
    int len = recv(clientfd, buf, BUF_SIZE, 0);
    

    if(len == 0)  // len = 0 means the client closed connection
    {
        close(clientfd);
        clients_list.remove(clientfd); //server remove the client
        printf("ClientID = %d closed.\n now there are %d client in the char room\n", clientfd, (int)clients_list.size());

        this->controller->clientClosed(clientfd);
    }
    else  //broadcast message
    {
        try {
            json j = json::parse(buf);
        
            this->controller->receiveMessage(clientfd, j);
        }
        catch (json::parse_error& e)
        {
            // output exception information
            std::cout << "message: " << e.what() << '\n'
                    << "exception id: " << e.id << '\n'
                    << "byte position of error: " << e.byte << std::endl;
        }
        
        // list<int>::iterator it;
        // for(it = clients_list.begin(); it != clients_list.end(); ++it) {
        //    if(*it != clientfd){
        //         if( send(*it, message, BUF_SIZE, 0) < 0 ) { perror("error"); exit(-1);}
        //    }
        // }
    }
    return len;
}