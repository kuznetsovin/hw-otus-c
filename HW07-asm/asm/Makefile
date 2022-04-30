AS = nasm  # NOTE: yasm тоже подойдёт

all: asm-prog

asm-prog: main.o
	$(CC) $^ -o $@

main.o: main.asm
	$(AS) -felf64 $^

clean:
	rm -f asm-prog *.o core

PHONY: clean
