all: s c

s: server.c
	cc -Wall -Wextra -pthread -o ./build/s server.c

c: client.c
	cc -Wall -Wextra -o ./build/c client.c

