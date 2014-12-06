#ifndef _procPerProto_
#define _procPerProto_

#include <arpa/inet.h>
#include <iostream>
#include <mutex>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "message.h"
#include "threadpool.h"
using namespace std;

typedef void (*function_pointer)(void*);
void* ftpApp(void* arg);
void* telnetApp(void* arg);
void* rdpApp(void* arg);
void* dnsApp(void* arg);
class procPerProto
{
public:
    // Struct for sending to pipes
    struct pipeStuff {
        int protocol_id;
        Message *msg;
    };

    //Struct to store arguements for the resquester threads
    typedef struct {
        // Array for pipe descriptor
        int pipe_d[2];
     
        //Pointer to mutex
        pthread_mutex_t* pipeMutex;
    } pipes;

    procPerProto(char out[], char in[]);
    ~procPerProto();
    
    void messageSend(Message* msg, int protocol_id);
    void comInit(char out[], char in[]);

    // Allocate structs for application level pipes
    pipes ftpPipeSend, telnetPipeSend, rdpPipeSend, dnsPipeSend;

    // Mutext locks for application level send pipes
    pthread_mutex_t ftpMutexSend;
    pthread_mutex_t telnetMutexSend;
    pthread_mutex_t rdpMutexSend;
    pthread_mutex_t dnsMutexSend;

private:
    ThreadPool* threadPool;

    static void* ethSend(void* arg);
    static void* ethIn(void* arg);
    static void* ipSend(void* arg);
    static void* ipIn(void* arg);
    static void* tcpSend(void* arg);
    static void* tcpIn(void* arg);
    static void* udpSend(void* arg);
    static void* udpIn(void* arg);
    static void* dnsSend(void* arg);
    static void* dnsIn(void* arg);
    static void* ftpSend(void* arg);
    static void* ftpIn(void* arg);
    static void* rdpSend(void* arg);
    static void* rdpIn(void* arg);
    static void* telnetSend(void* arg);
    static void* telnetIn(void* arg);


    static void* messageIn(void* arg);

    pipes ftpPipeIn, telnetPipeIn, rdpPipeIn, dnsPipeIn;
    pipes tcpPipeSend, tcpPipeIn, udpPipeSend, udpPipeIn;
    pipes ipPipeSend, ipPipeIn, ethPipeSend, ethPipeIn;


    pthread_mutex_t ethMutexSend;
    pthread_mutex_t ethMutexIn;
    pthread_mutex_t ipMutexSend;
    pthread_mutex_t ipMutexIn;
    pthread_mutex_t tcpMutexSend;
    pthread_mutex_t tcpMutexIn;
    pthread_mutex_t udpMutexSend;
    pthread_mutex_t udpMutexIn;
    pthread_mutex_t dnsMutexIn;
    pthread_mutex_t ftpMutexIn;
    pthread_mutex_t rdpMutexIn;
    pthread_mutex_t telnetMutexIn;

    typedef struct {
        int hlp;
     
        char oi[8];

        int m_size;
    } ethHeader;

    typedef struct {
        int hlp;
     
        char oi[12];

        int m_size;
    } ipHeader;


    typedef struct {
        int hlp;
     
        char oi[4];

        int m_size;
    } tcpHeader;

    typedef struct {
        int hlp;
     
        char oi[4];

        int m_size;
    } udpHeader;

    typedef struct {
        int hlp;
     
        char oi[8];

        int m_size;
    } dnsHeader;

    typedef struct {
        int hlp;
     
        char oi[8];

        int m_size;
    } ftpHeader;
    
    typedef struct {
        int hlp;
     
        char oi[12];

        int m_size;
    } rdpHeader;

    typedef struct {
        int hlp;
     
        char oi[8];

        int m_size;
    } telnetHeader;

    int inSock, outSock;

    char* mutSendPort;
    char* mutRecvPort;
};

#endif