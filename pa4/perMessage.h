#ifndef _procPerMessage_
#define _procPerMessage_

#include <map>
#include <semaphore.h>
#include <string.h>
#include <mutex>
#include <iostream>
#include "message.h"
#include "threadpool.h"

typedef void (*function_pointer)(void*);

void* ftpApp(void* arg);
void* telnetApp(void* arg);
void* rdpApp(void* arg);
void* dnsApp(void* arg);

class procPerMessage
{
public:
    procPerMessage();
    procPerMessage(char* outPort, char* inPort);
    ~procPerMessage();
    
    void ethernetSend(int protocol_id, Message* msg);
    static void ethernetIn(void* arg);
    void ipSend(int protocol_id, Message* msg);
    static void ipIn(Message* msg);
    void tcpSend(int protocol_id, Message* msg);
    static void tcpIn(Message* msg);
    void udpSend(int protocol_id, Message* msg);
    static void udpIn(Message* msg);
    void ftpSend(int protocol_id, Message* msg);
    static void ftpIn(Message* msg);
    void telnetSend(int protocol_id, Message* msg);
    static void telnetIn(Message* msg);
    void rdpSend(int protocol_id, Message* msg);
    static void rdpIn(Message* msg);
    void dnsSend(int protocol_id, Message* msg);
    static void dnsIn(Message* msg);

private:
    ThreadPool* threadPool;
    static void* seeUdp(void* arg);
    char* mutSendPort;
    char* mutRecvPort;
};

#endif
