CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -O2 -Isrc

SRC = src/sort.c src/insertionSort.c src/quickSort.c src/blockSort.c src/bench.c
MAIN_SRC = src/main.c
TEST_SRC = tests/test_sort.c

all: main.out test.out

main.out: $(SRC) $(MAIN_SRC)
	$(CC) $(CFLAGS) -o main.out $(SRC) $(MAIN_SRC)

test.out: $(SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) -o test.out $(SRC) $(TEST_SRC)

run: main.out
	./main.out

test: test.out
	./test.out

clean:
	rm -f main.out test.out