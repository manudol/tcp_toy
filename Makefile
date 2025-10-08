all: s c

$(shell mkdir -p build)


utils.o: utils.c utils.h
	cc -Wall -Wextra -c utils.c -o ./build/utils.o

json.o: json/json.c json/json.h utils.h
	cc -Wall -Wextra -c ./json/json.c -o ./build/json.o

server.o: server.c json/json.h
	cc -Wall -Wextra -pthread -I. -c server.c -o ./build/server.o

client.o: client.c json/json.h
	cc -Wall -Wextra -I. -c client.c -o ./build/client.o


s: server.o json.o utils.o
	cc -pthread -o ./build/s ./build/server.o ./build/json.o ./build/utils.o -ljson-c

c: client.o json.o utils.o
	cc -o ./build/c ./build/client.o ./build/json.o ./build/utils.o -ljson-c

clean:
	rm -f ./build/*
