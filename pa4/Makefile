CFLG=-Wall -O0 -w -g -std=c++11
LIBS=-lpthread
CC=g++

# Main 
all: perMessageHostOne perMessageHostTwo perProtoHostOne perProtoHostTwo

# Compile rules
.c.o:
	gcc -c $(CFLG) $<
.cpp.o:
	g++ -c $(CFLG) $<

perMessageHostOne:perMessageHostOne.o
	$(CC) -o $@ $(CFLG) perMessage.cpp threadpool.cpp message.cpp $^ $(LIBS)

perMessageHostTwo:perMessageHostTwo.o
	$(CC) -o $@ $(CFLG) perMessage.cpp threadpool.cpp message.cpp $^ $(LIBS)

perProtoHostOne:perProtoHostOne.o
	$(CC) -o $@ $(CFLG) perProto.cpp threadpool.cpp message.cpp $^ $(LIBS)

perProtoHostTwo:perProtoHostTwo.o
	$(CC) -o $@ $(CFLG) perProto.cpp threadpool.cpp message.cpp $^ $(LIBS)	

clean:
	rm -f perMessageTest perMessageHostOne perMessageHostTwo perProtoHostOne perProtoHostTwo *.o *.a
