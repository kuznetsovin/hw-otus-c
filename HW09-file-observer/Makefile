.PHONY: all test clean

all: build build_client

build:
	gcc src/main.c -Wall -Wextra -Wpedantic -std=c11 -o file-observer

build_client:
	gcc src/test_client.c -o test_client

run:
	touch test.txt && ./file-observer test.txt sock.socket

daemon:
	touch test.txt && ./file-observer -d test.txt sock.socket

valgrind:
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./file-observer test.txt sock.socket
clean:
	rm -f file-observer
