.PHONY: all test clean

all: test build clean

test:
	gcc src/zipjpeg/zipjpeg.c src/zipjpeg/zipjpeg_test.c ../assets/Unity/src/unity.c -Wall -Wextra -Wpedantic -std=c11 -o test_runner && ./test_runner

build:
	mkdir bin && gcc src/main.c src/zipjpeg/zipjpeg.c -Wall -Wextra -Wpedantic -std=c11 -o bin/zipjpeg

clean:
	rm -f test_runner
