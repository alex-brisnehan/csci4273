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
        pthread_mutex_t* pipe_mutex;
    } pipes;

    procPerProto(char in[], char out[]);
    ~procPerProto();
    
    void messageSend(Message* msg, int protocol_id);
    void start_com(char in[], char[]);

    // Allocate structs for application level pipes
    pipes ftpPipeSend, telnetPipeSend, rdpPipeSend, dnsPipeSend;

    // Mutext locks for application level send pipes
    pthread_mutex_t ftpMutexSend;
    pthread_mutex_t telnetMutexSend;
    pthread_mutex_t rdpMutexSend;
    pthread_mutex_t dnsMutexSend;

private:
    ThreadPool* m_thread_pool;

    // Functions for protocols
    static void* ethSend(void* arg);
    static void* ethIn(void* arg);
    static void* ipSend(void* arg);
    static void* ipIn(void* arg);
    static void* tcpSend(void* arg);
    static void* tcpIn(void* arg);
    static void* udpSend(void* arg);
    static void* udpIn(void* arg);
    static void* ftpSend(void* arg);
    static void* ftpIn(void* arg);
    static void* telnetSend(void* arg);
    static void* telnetIn(void* arg);
    static void* rdpSend(void* arg);
    static void* rdpIn(void* arg);
    static void* dnsSend(void* arg);
    static void* dnsIn(void* arg);

    // Functions for receiving messages
    static void* messageIn(void* arg);

    // Allocate structs for pipes
    pipes ftpPipeIn, telnetPipeIn, rdpPipeIn, dnsPipeIn;
    pipes tcpPipeSend, tcpPipeIn, udpPipeSend, udpPipeIn;
    pipes ipPipeSend, ipPipeIn, ethPipeSend, ethPipeIn;

    // Allocate mutex locks for pipes
    pthread_mutex_t ftpMutexIn;
    pthread_mutex_t telnetMutexIn;
    pthread_mutex_t rdpMutexIn;
    pthread_mutex_t dnsMutexIn;
    pthread_mutex_t tcpMutexSend;
    pthread_mutex_t tcpMutexIn;
    pthread_mutex_t udpMutexSend;
    pthread_mutex_t udpMutexIn;
    pthread_mutex_t ipMutexSend;
    pthread_mutex_t ipMutexIn;
    pthread_mutex_t ethSend_mut;
    pthread_mutex_t ethMutexIn;

    //Struct to store arguements for the resquester threads
    typedef struct {
        // Int for higher level protocol
        int hlp;
     
        // Char array for other info
        char oi[12];

        // Int for size of message without header
        int m_size;
    } IP_header;

    //Struct to store arguements for the resquester threads
    typedef struct {
        // Int for higher level protocol
        int hlp;
     
        // Char array for other info
        char oi[8];

        // Int for size of message without header
        int m_size;
    } ethHeader;

    //Struct to store arguements for the resquester threads
    typedef struct {
        // Int for higher level protocol
        int hlp;
     
        // Char array for other info
        char oi[4];

        // Int for size of message without header
        int m_size;
    } TCP_header;

    //Struct to store arguements for the resquester threads
    typedef struct {
        // Int for higher level protocol
        int hlp;
     
        // Char array for other info
        char oi[4];

        // Int for size of message without header
        int m_size;
    } UDP_header;

    //Struct to store arguements for the resquester threads
    typedef struct {
        // Int for higher level protocol
        int hlp;
     
        // Char array for other info
        char oi[12];

        // Int for size of message without header
        int m_size;
    } RDP_header;

    //Struct to store arguements for the resquester threads
    typedef struct {
        // Int for higher level protocol
        int hlp;
     
        // Char array for other info
        char oi[8];

        // Int for size of message without header
        int m_size;
    } DNS_header;

    //Struct to store arguements for the resquester threads
    typedef struct {
        // Int for higher level protocol
        int hlp;
     
        // Char array for other info
        char oi[8];

        // Int for size of message without header
        int m_size;
    } ftpHeader;

    //Struct to store arguements for the resquester threads
    typedef struct {
        // Int for higher level protocol
        int hlp;
     
        // Char array for other info
        char oi[8];

        // Int for size of message without header
        int m_size;
    } telnetHeader;

    // Variable for storing the udp socket for the virtual network
    int inSock, outSock;

    char* mutSendPort;
    char* mutRecvPort;
};

#endif