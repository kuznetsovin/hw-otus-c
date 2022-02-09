.PHONY: all test clean

all: test build clean

test:
	gcc src/encoding/encoder.c src/encoding/encoder_test.c ../assets/Unity/src/unity.c -fsanitize=address -fno-omit-frame-pointer -Wall -Wextra -Wpedantic -std=c11 -o test_runner && ./test_runner

build:
	mkdir bin && gcc src/main.c src/encoding/encoder.c -Wall -Wextra -Wpedantic -std=c11 -o bin/utf-encoder

clean:
	rm -f test_runner
