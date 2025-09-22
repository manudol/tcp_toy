all: s c

utils.o: utils.c utils.h
	cc -Wall -Wextra -c utils.c -o ./build/utils.o

server.o: server.c utils.h
	cc -Wall -Wextra -pthread -c server.c -o ./build/server.o

client.o: client.c utils.h
	cc -Wall -Wextra -c client.c -o ./build/client.o


s: server.o utils.o
	cc -pthread -o ./build/s ./build/server.o ./build/utils.o

c: client.o
	cc -o ./build/c ./build/client.o ./build/utils.o

