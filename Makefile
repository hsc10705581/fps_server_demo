all: server

server: main.o epoll_server.o controller.o player.o vector3.o database.o auth.o sender.o 
	g++ -o server main.o epoll_server.o controller.o player.o vector3.o database.o auth.o sender.o -lz -lcrypt -lmysqlclient -pthread

main.o: main.cpp
	g++ -c -Wall -std=c++11 main.cpp epoll_server.h

epoll_server.o: epoll_server.cpp
	g++ -c -Wall -std=c++11 epoll_server.cpp epoll_server.h

controller.o: controller.cpp
	g++ -c -Wall -std=c++11 controller.cpp controller.h

player.o: player.cpp
	g++ -c -Wall -std=c++11 player.cpp player.h common.h

vector3.o: vector3.cpp
	g++ -c -Wall -std=c++11 vector3.cpp common.h

auth.o: auth.cpp
	g++ -c -Wall -std=c++11 auth.cpp auth.h database.h

database.o: database.cpp
	g++ -c -Wall -std=c++11 database.cpp database.h

sender.o: sender.cpp
	g++ -c -Wall -std=c++11 sender.cpp sender.h

clean:
	rm *.o rm *.h.gch

clean_header:
	rm *.h.gch
