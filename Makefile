all: s c

$(shell mkdir -p build)

json.o: json/json.c json/json.h
	cc -Wall -Wextra -c ./json/json.c -o ./build/json.o

utils.o: utils.c utils.h
	cc -Wall -Wextra -c utils.c -o ./build/utils.o

server.o: server.c utils.h json/json.h
	cc -Wall -Wextra -pthread -I. -c server.c -o ./build/server.o

client.o: client.c utils.h json/json.h
	cc -Wall -Wextra -I. -c client.c -o ./build/client.o


s: server.o json.o utils.o
	cc -pthread -ljson-c -o ./build/s ./build/server.o ./build/utils.o ./build/json.o

c: client.o json.o utils.o
	cc -ljson-c -o ./build/c ./build/client.o ./build/utils.o ./build/json.o

clean:
	rm -f ./build/*
