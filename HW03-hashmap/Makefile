.PHONY: all test clean

all: test build clean

test:
	gcc src/hashmap/hashmap.c src/hashmap/hashmap_test.c ../assets/Unity/src/unity.c -fsanitize=address -fno-omit-frame-pointer -Wall -Wextra -Wpedantic -std=c11 -o test_runner_hm && ./test_runner_hm
	gcc src/wcounter/wcounter.c src/hashmap/hashmap.c src/wcounter/wcounter_test.c ../assets/Unity/src/unity.c -fsanitize=address -fno-omit-frame-pointer -Wall -Wextra -Wpedantic -std=c11 -o test_runner_wc && ./test_runner_wc

build:
	mkdir bin && gcc src/main.c src/hashmap/hashmap.c src/wcounter/wcounter.c -Wall -Wextra -Wpedantic -std=c11 -o bin/word-counter

clean:
	rm -f test_runner_hm
	rm -f test_runner_wc
