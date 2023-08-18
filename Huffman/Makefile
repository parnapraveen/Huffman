CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic $(shell pkg-config --cflags gmp)
LFLAGS = $(shell pkg-config)

all: encode decode

encode: encode.o node.o pq.o code.o io.o stack.o huffman.o
	$(CC) -o $@ $^ $(LFLAGS)

decode: decode.o node.o pq.o code.o io.o stack.o huffman.o
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f encode decode *.o

format:
	clang-format -i -style=file *.[ch]

