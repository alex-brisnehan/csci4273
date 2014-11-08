CFLG=-O3 -Wall -w -std=c++11
LIBS=-lpthread
CC=g++

# Main target
all: Message ThreadPool EventScheduler

Message:
	$(CC) -o Message $(CFLG) driver3.cc Message.cpp $(LIBS)

ThreadPool:
	$(CC) -o ThreadPool $(CFLG) driver1.cc ThreadPool.cpp $(LIBS)

EventScheduler:
	$(CC) -o EventScheduler $(CFLG) driver2.cc EventScheduler.cpp ThreadPool.cpp $(LIBS)


clean:
	rm -f Message ThreadPool EventScheduler *.o *.a

