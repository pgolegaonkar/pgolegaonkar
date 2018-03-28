CC = gcc
CFLAGS = -Wall -lpthread -O2

clean:
	rm -rf *.o MyDiskBench

all: clean
	$(CC) $(CFLAGS) -pthread MyDiskBench.c -o MyDiskBench

