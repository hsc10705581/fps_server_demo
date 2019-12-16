all: server

server: main.o epoll_server.o controller.o
	g++ -o server main.o epoll_server.o controller.o -lz -lcrypt

main.o: main.cpp
	g++ -c -Wall -std=c++11 main.cpp epoll_server.h

epoll_server.o: epoll_server.cpp
	g++ -c -Wall -std=c++11 epoll_server.cpp epoll_server.h

controller.o: controller.cpp
	g++ -c -Wall -std=c++11 controller.cpp controller.h

vector3.o: vector3.cpp
	g++ -c -Wall -std=c++11 vector3.cpp common.h

clean:
	rm *.o