#include "epoll_server.h"

sem_t mutex;
EpollServer * server;
void *sendThread(void *vargp);

int main(int argc, char *argv[])
{
    sem_init(&mutex, 0, 1);
    server = new EpollServer();
    // parent thread used for receive & child thread used for send
    //int pid = fork();
    //if(pid < 0) { perror("fork error"); exit(-1); }
    pthread_t tid;
    pthread_create(&tid, NULL, sendThread, (void *)0);
    while(1)
    {
        if (!server->mainloop(mutex))
            break;
    }
    // while(1)
    // {
    //     if (!server->mainloop())
    //         break;
    // }
    close(server->getListener()); //关闭socket
    close(server->getEpfd());    //关闭内核   不在监控这些注册事件是否发生
    
    return 0;
}

void *sendThread(void *vargp)
{
    printf("send loop begin\n");
    while(1)
    {
        if (!server->sendloop(mutex))
            break;
    }
    printf("send loop end\n");
    return NULL;
}
