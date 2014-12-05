#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "perProto.h"
#include "udpUtils.cpp"

using namespace std;

struct pipeStuff {
    int protocol_id;
    Message *msg;
};

procPerProto::procPerProto(char in[], char out[]){
    //Start up the communication
    start_com(in, out);


    // Create pipes. Its a bunch because there are two for each
    pipe(ftpPipeSend.pipe_d);
    pipe(ftpPipeIn.pipe_d);

    pipe(telnetPipeSend.pipe_d);
    pipe(telnetPipeIn.pipe_d);

    pipe(rdpPipeSend.pipe_d);
    pipe(rdpPipeIn.pipe_d);

    pipe(dnsPipeSend.pipe_d);
    pipe(dnsPipeIn.pipe_d);

    pipe(tcpPipeSend.pipe_d);
    pipe(tcpPipeIn.pipe_d);

    pipe(udpPipeSend.pipe_d);
    pipe(udpPipeIn.pipe_d);

    pipe(ipPipeSend.pipe_d);
    pipe(ipPipeIn.pipe_d);

    pipe(ethPipeSend.pipe_d);
    pipe(ethPipeIn.pipe_d);


    // Mutex gibberish to initialize locks for pipes
    ftpMutexSend = PTHREAD_MUTEX_INITIALIZER;
    ftpPipeSend.pipe_mutex = &ftpMutexSend;
    ftpMutexIn = PTHREAD_MUTEX_INITIALIZER;
    ftpPipeIn.pipe_mutex = &ftpMutexIn;

    telnetMutexSend = PTHREAD_MUTEX_INITIALIZER;
    telnetPipeSend.pipe_mutex = &telnetMutexSend;
    telnetMutexIn = PTHREAD_MUTEX_INITIALIZER;
    telnetPipeIn.pipe_mutex = &telnetMutexIn;

    rdpMutexSend = PTHREAD_MUTEX_INITIALIZER;
    rdpPipeSend.pipe_mutex = &rdpMutexSend;
    rdpMutexIn = PTHREAD_MUTEX_INITIALIZER;
    rdpPipeIn.pipe_mutex = &rdpMutexIn;

    dnsMutexSend = PTHREAD_MUTEX_INITIALIZER;
    dnsPipeSend.pipe_mutex = &dnsMutexSend;
    dnsMutexIn = PTHREAD_MUTEX_INITIALIZER;
    dnsPipeIn.pipe_mutex = &dnsMutexIn;

    tcpMutexSend = PTHREAD_MUTEX_INITIALIZER;
    tcpPipeSend.pipe_mutex = &tcpMutexSend;
    tcpMutexIn = PTHREAD_MUTEX_INITIALIZER;
    tcpPipeIn.pipe_mutex = &tcpMutexIn;

    udpMutexSend = PTHREAD_MUTEX_INITIALIZER;
    udpPipeSend.pipe_mutex = &udpMutexSend;
    udpMutexIn = PTHREAD_MUTEX_INITIALIZER;
    udpPipeIn.pipe_mutex = &udpMutexIn;

    ipMutexSend = PTHREAD_MUTEX_INITIALIZER;
    ipPipeSend.pipe_mutex = &ipMutexSend;
    ipMutexIn = PTHREAD_MUTEX_INITIALIZER;
    ipPipeIn.pipe_mutex = &ipMutexIn;

    ethSend_mut = PTHREAD_MUTEX_INITIALIZER;
    ethPipeSend.pipe_mutex = &ethSend_mut;
    ethMutexIn = PTHREAD_MUTEX_INITIALIZER;
    ethPipeIn.pipe_mutex = &ethMutexIn;

    m_thread_pool = new ThreadPool(16);

    // Create threads
    pthread_t tid_1, tid_2, tid_3, tid_4, tid_5, tid_6, tid_7, tid_8, tid_9;
    pthread_t tid_10, tid_11, tid_12, tid_13, tid_14, tid_15, tid_16, tid_17;

    int err;

    // Messages thread
    err = pthread_create(&tid_17, NULL, procPerProto::messageIn, (void*) this);
    if (err != 0) {
        cout << "messageIn() failed: " << err << endl;
        exit(1);
    }

    // Ethernet Recv thread
    err = pthread_create(&tid_1, NULL, procPerProto::ethIn, (void*) this);
    if (err != 0) {
        cout << "ethIn() failed: " << err << endl;
        exit(1);
    }

    // Ethernet Send Thread
    err = pthread_create(&tid_2, NULL, procPerProto::ethSend, (void*) this);
    if (err != 0) {
        cout << "ethSend() failed: " << err << endl;
        exit(1);
    }

    // IP Recv Thread
    err = pthread_create(&tid_3, NULL, procPerProto::ipIn, (void*) this);
    if (err != 0) {
        cout << "ipIn() failed: " << err << endl;
        exit(1);
    }

    // IP Send Thread
    err = pthread_create(&tid_4, NULL, procPerProto::ipSend, (void*) this);
    if (err != 0) {
        cout << "ipSend() failed: " << err << endl;
        exit(1);
    }

    // UDP Recv Thread
    err = pthread_create(&tid_5, NULL, procPerProto::udpIn, (void*) this);
    if (err != 0) {
        cout << "udpIn() failed: " << err << endl;
        exit(1);
    }

    // UDP Send Thread
    err = pthread_create(&tid_6, NULL, procPerProto::udpSend, (void*) this);
    if (err != 0) {
        cout << "udpIn() failed: " << err << endl;
        exit(1);
    }

    // TCP Recv Thread
    err = pthread_create(&tid_7, NULL, procPerProto::tcpIn, (void*) this);
    if (err != 0) {
        cout << "tcpIn() failed: " << err << endl;
        exit(1);
    }

    // TCP Send Thread
    err = pthread_create(&tid_8, NULL, procPerProto::tcpSend, (void*) this);
    if (err != 0) {
        cout << "tcpSend() failed: " << err << endl;
        exit(1);
    }

    // FTP Recv Thread
    err = pthread_create(&tid_9, NULL, procPerProto::ftpIn, (void*) this);
    if (err != 0) {
        cout << "ftpIn() failed: " << err << endl;
        exit(1);
    }

    // FTP Send Thread
    err = pthread_create(&tid_10, NULL, procPerProto::ftpSend, (void*) this);
    if (err != 0) {
        cout << "ftpSend() failed: " << err << endl;
        exit(1);
    }

    // Telnet Recv Thread
    err = pthread_create(&tid_11, NULL, procPerProto::telnetIn, (void*) this);
    if (err != 0) {
        cout << "telnetIn() failed: " << err << endl;
        exit(1);
    }

    // Telnet Send Thread
    err = pthread_create(&tid_12, NULL, procPerProto::telnetSend, (void*) this);
    if (err != 0) {
        cout << "telnetSend() failed: " << err << endl;
        exit(1);
    }

    // RDP Recv Thread
    err = pthread_create(&tid_13, NULL, procPerProto::rdpIn, (void*) this);
    if (err != 0) {
        cout << "rdpIn() failed: " << err << endl;
        exit(1);
    }

    // RDP Send Thread
    err = pthread_create(&tid_14, NULL, procPerProto::rdpSend, (void*) this);
    if (err != 0) {
        cout << "rdpSend() failed: " << err << endl;
        exit(1);
    }

    // DNS Recv Thread
    err = pthread_create(&tid_15, NULL, procPerProto::dnsIn, (void*) this);
    if (err != 0) {
        cout << "dnsIn() failed: " << err << endl;
        exit(1);
    }

    // DNS Send Thread
    err = pthread_create(&tid_16, NULL, procPerProto::dnsSend, (void*) this);
    if (err != 0) {
        cout << "dnsSend() failed: " << err << endl;
        exit(1);
    }

}

procPerProto::~procPerProto()
{
    delete m_thread_pool;
}

void procPerProto::start_com(char in[], char out[]){
    inSock = udpSocket(in);
    mutSendPort = out;

}

void* procPerProto::messageIn(void* arg)
{
    int mess;
    procPerProto* perProto = (procPerProto*) arg;
    socklen_t len;
    struct sockaddr_in clientAddress;



    while (1) {
        char* msg_buf = new char[1024];
        memset(msg_buf, 0, 1024);
        len = sizeof(clientAddress);

        // Socket got message. Read it.
        mess = recvfrom(perProto->inSock, msg_buf, 1024, 0, (struct sockaddr *)&clientAddress, &len);
        Message* msg = new Message(msg_buf, mess);

        //Need a pipe for this
        pipeStuff outPipe;
        outPipe.protocol_id = 0;
        outPipe.msg = msg;
        
        // Lock Pipe to send to ethernet
        pthread_mutex_lock(perProto->ethPipeIn.pipe_mutex);

        // Write to ethernet in pipe
        write(perProto->ethPipeIn.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

        //Unlock
        pthread_mutex_unlock(perProto->ethPipeIn.pipe_mutex);
    }
}

void procPerProto::messageSend(Message* msg, int protocol_id){
    //Is it coming through FTP?
    if (protocol_id == 5) {
        pipeStuff ftp;
        ftp.protocol_id = 5;
        ftp.msg = msg;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(ftpPipeSend.pipe_mutex);

        // Write to ftp send pipe
        write(ftpPipeSend.pipe_d[1], (char*) &ftp, sizeof(pipeStuff));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(ftpPipeSend.pipe_mutex);
    }

    else if (protocol_id == 6){
        pipeStuff tel;
        tel.protocol_id = 6;
        tel.msg = msg;
        
        // Acquire mutex lock on pipe
        pthread_mutex_lock(telnetPipeSend.pipe_mutex);

        // Write to ftp send pipe
        write(telnetPipeSend.pipe_d[1], (char*) &tel, sizeof(pipeStuff));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(telnetPipeSend.pipe_mutex);
    }

    else if (protocol_id == 7){
        pipeStuff rdp;
        rdp.protocol_id = 7;
        rdp.msg = msg;
        
        // Acquire mutex lock on pipe
        pthread_mutex_lock(rdpPipeSend.pipe_mutex);

        // Write to ftp send pipe
        write(rdpPipeSend.pipe_d[1], (char*) &rdp, sizeof(pipeStuff));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(rdpPipeSend.pipe_mutex);
    }

    else if (protocol_id == 8){
        pipeStuff dns;
        dns.protocol_id = 8;
        dns.msg = msg;

        // For testing
        //char* test2 = new char[1024];
        //dns.msg->msgFlat(test2);
        //cout << "Message in pipeStuff struct for dns: " << test2 << endl;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(dnsPipeSend.pipe_mutex);

        // Write to ftp send pipe
        write(dnsPipeSend.pipe_d[1], (char*) &dns, sizeof(pipeStuff));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(dnsPipeSend.pipe_mutex);
    }

    else{
        cout << "Invalid protocol_id" << endl;
    }
}

void* procPerProto::ethSend(void* arg){
    procPerProto* perProto = (procPerProto*) arg;
    struct sockaddr_in serverAddress;
    socklen_t len;
    struct hostent *phe;    // pointer to host information entry

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;

    char* host = "localhost";

    // Map port number (char string) to port number (int)
    if ((serverAddress.sin_port = htons((unsigned short)atoi(perProto->mutSendPort))) == 0)
        errexit("can't get \"%s\" port number\n", perProto->mutSendPort);

    // Map host name to IP address, allowing for dotted decimal
    if ( (phe = gethostbyname(host)) )
    {
        memcpy(&serverAddress.sin_addr, phe->h_addr, phe->h_length);
    }
    else if ( (serverAddress.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
        errexit("can't get \"%s\" host entry\n", host);

    // Allocate a socket
    int udpSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSock < 0)
        errexit("can't create socket: %s\n", strerror(errno));
    
    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Mutext unlock on read pipe
        pthread_mutex_unlock(perProto->ethPipeSend.pipe_mutex);

        // Wait until read succeeds
        read(perProto->ethPipeSend.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Mutext lock on read pipe to prevent race conditions
        pthread_mutex_lock(perProto->ethPipeSend.pipe_mutex);

        // Store message in variable
        msg = seePipe->msg;

        // Create new header
        ethHeader* h = (ethHeader *) malloc( sizeof(ethHeader));
        h->hlp = seePipe->protocol_id;
        h->m_size = msg->msgLen();

        // For testing
        //cout << "HLP in eth send: " << h->hlp << endl;
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(ethHeader));

        // Flaten message to buffer
        char msg_buf[1024];
        memset(&msg_buf, 0, sizeof(msg_buf));
        msg->msgFlat(msg_buf);

        // Sent message to network
        if (sendto(udpSock, msg_buf, msg->msgLen(), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
        printf("Error with sendto %s\n", strerror(errno));

        // For testing
        //cout << "Message sent over network to peer" << endl;
    }


}

void* procPerProto::ethIn(void* arg){
    procPerProto* perProto = (procPerProto*) arg;
    
    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Mutext unlock on read pipe
        pthread_mutex_unlock(perProto->ethPipeIn.pipe_mutex);

        // Wait to read message from pipe
        read(perProto->ethPipeIn.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Mutext lock on read pipe to prevent race conditions
        pthread_mutex_lock(perProto->ethPipeIn.pipe_mutex);

        // For testing
        //cout << "Message read from receive pipe in eth recv" << endl;

        // Strip headers
        msg = seePipe->msg;
        ethHeader* stripped = (ethHeader*)msg->msgStripHdr(sizeof(ethHeader));
        int protocol_id = stripped->hlp;

        // For testing
        //cout << "Protocol ID stripped in eth header" << protocol_id << endl;
        //cout << "Message size stripped in eth header" << stripped->m_size << endl;

        // Create new pipe unit
        pipeStuff outPipe;
        outPipe.protocol_id = 1;
        outPipe.msg = msg;

        if (protocol_id == 2){
            // Lock mutex
            pthread_mutex_lock(perProto->ipPipeIn.pipe_mutex);

            // Write to pipe
            write(perProto->ipPipeIn.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

            // Unlock mutex
            pthread_mutex_unlock(perProto->ipPipeIn.pipe_mutex);
        }
        else{
            cout << "Error invalid protocol going up from eth" << endl;
        }
    }
}

void* procPerProto::ipSend(void* arg){
    procPerProto* perProto = (procPerProto*) arg;

    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Mutext unlock on read pipe
        pthread_mutex_unlock(perProto->ipPipeSend.pipe_mutex);

        // Wait for message to send
        read(perProto->ipPipeSend.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Mutext lock on read pipe to prevent race conditions
        pthread_mutex_lock(perProto->ipPipeSend.pipe_mutex);

        // Store message in variable
        msg = seePipe->msg;

        // For testing
        //UDP_header* h2 = (UDP_header*)msg->msgStripHdr(sizeof(UDP_header));
        //cout << "Size in stripped header for UDP: " << h2->m_size << endl;
        //cout << "HLP in stripped header for UDP: " << h2->hlp << endl;

        // Create new header
        IP_header* h = (IP_header *) malloc( sizeof(IP_header));
        h->hlp = seePipe->protocol_id;
        h->m_size = msg->msgLen();
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(IP_header));

        // Build new pipe unit
        pipeStuff outPipe;
        outPipe.protocol_id = 2;
        outPipe.msg = msg;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(perProto->ethPipeSend.pipe_mutex);

        // Write to eth send pipe
        write(perProto->ethPipeSend.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(perProto->ethPipeSend.pipe_mutex);
    }
}

void* procPerProto::ipIn(void* arg){
    procPerProto* perProto = (procPerProto*) arg;
    
    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Mutext unlock on read pipe
        pthread_mutex_unlock(perProto->ipPipeIn.pipe_mutex);

        // Read from recv pipe
        read(perProto->ipPipeIn.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Mutext lock on read pipe to prevent race conditions
        pthread_mutex_lock(perProto->ipPipeIn.pipe_mutex);

        // Add header to message
        msg = seePipe->msg;
        IP_header* stripped = (IP_header*)msg->msgStripHdr(sizeof(IP_header));
        int protocol_id = stripped->hlp;

        // Create new pipe unit
        pipeStuff outPipe;
        outPipe.protocol_id = 2;
        outPipe.msg = msg;

        if (protocol_id == 3){
            // Lock mutex
            pthread_mutex_lock(perProto->tcpPipeIn.pipe_mutex);

            // Write to pipe
            write(perProto->tcpPipeIn.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

            // Unlock mutex
            pthread_mutex_unlock(perProto->tcpPipeIn.pipe_mutex);
        }
        else if (protocol_id == 4){
            // Lock mutex
            pthread_mutex_lock(perProto->udpPipeIn.pipe_mutex);

            // Write to pipe
            write(perProto->udpPipeIn.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

            // Unlock mutex
            pthread_mutex_unlock(perProto->udpPipeIn.pipe_mutex);
        }
        else{
            cout << "Error invalid protocol going up from ip" << endl;
        }
    }
}

void* procPerProto::tcpSend(void* arg){
    procPerProto* perProto = (procPerProto*) arg;

    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Mutext unlock on read pipe
        pthread_mutex_unlock(perProto->tcpPipeSend.pipe_mutex);

        // Wait for message to send
        read(perProto->tcpPipeSend.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Mutext lock on read pipe to prevent race conditions
        pthread_mutex_lock(perProto->tcpPipeSend.pipe_mutex);

        // Store message in variable
        msg = seePipe->msg;

        // Create new header
        TCP_header* h = (TCP_header *) malloc( sizeof(TCP_header));
        h->hlp = seePipe->protocol_id;
        h->m_size = msg->msgLen();
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(TCP_header));

        // Build new pipe unit
        pipeStuff outPipe;
        outPipe.protocol_id = 3;
        outPipe.msg = msg;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(perProto->ipPipeSend.pipe_mutex);

        // Write to eth send pipe
        write(perProto->ipPipeSend.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(perProto->ipPipeSend.pipe_mutex);
    }
}

void* procPerProto::tcpIn(void* arg){
    procPerProto* perProto = (procPerProto*) arg;
    
    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Mutext unlock on read pipe
        pthread_mutex_unlock(perProto->tcpPipeIn.pipe_mutex);

        // Read from recv pipe
        read(perProto->tcpPipeIn.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Mutext lock on read pipe to prevent race conditions
        pthread_mutex_lock(perProto->tcpPipeIn.pipe_mutex);

        // Strip headers
        msg = seePipe->msg;
        TCP_header* stripped = (TCP_header*)msg->msgStripHdr(sizeof(TCP_header));
        int protocol_id = stripped->hlp;

        // Strip headers
        //msg = seePipe->msg;
        //int protocol_id = atoi(msg->msgStripHdr(2));
        //msg->msgStripHdr(6);

        // Create new pipe unit
        pipeStuff outPipe;
        outPipe.protocol_id = 3;
        outPipe.msg = msg;

        if (protocol_id == 5){
            // Lock mutex
            pthread_mutex_lock(perProto->ftpPipeIn.pipe_mutex);

            // Write to pipe
            write(perProto->ftpPipeIn.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

            // Unlock mutex
            pthread_mutex_unlock(perProto->ftpPipeIn.pipe_mutex);
        }
        else if (protocol_id == 6){
            // Lock mutex
            pthread_mutex_lock(perProto->telnetPipeIn.pipe_mutex);

            // Write to pipe
            write(perProto->telnetPipeIn.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

            // Unlock mutex
            pthread_mutex_unlock(perProto->telnetPipeIn.pipe_mutex);
        }
        else{
            cout << "Error invalid protocol going up from tcp" << endl;
        }
    }
}

void* procPerProto::udpSend(void* arg){
    procPerProto* perProto = (procPerProto*) arg;

    // For testing
    //cout << "Waiting for UDP message to send" << endl;

    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Mutext unlock on read pipe
        pthread_mutex_unlock(perProto->udpPipeSend.pipe_mutex);

        // Wait for message to send
        read(perProto->udpPipeSend.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Mutext lock on read pipe to prevent race conditions
        pthread_mutex_lock(perProto->udpPipeSend.pipe_mutex);

        // For testing
        //cout << "UDP message received to send" << endl;

        // Store message in variable
        msg = seePipe->msg;

        // For testing
        //DNS_header* h2 = (DNS_header*)msg->msgStripHdr(sizeof(DNS_header));
        //cout << "Size in stripped header: " << h2->m_size << endl;

        // Create new header
        UDP_header* h = (UDP_header *) malloc( sizeof(UDP_header));
        h->hlp = seePipe->protocol_id;
        h->m_size = msg->msgLen();

        // For testing
        //cout << "Protocol ID passed to UDP send: " << h->hlp << endl;
        //cout << "HLP in stripped header for pass to UDP send: " << h2->hlp << endl;
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(UDP_header));

        // Build new pipe unit
        pipeStuff outPipe;
        outPipe.protocol_id = 4;
        outPipe.msg = msg;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(perProto->ipPipeSend.pipe_mutex);

        // Write to eth send pipe
        write(perProto->ipPipeSend.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

        // For testing
        //cout << "UDP message written to IP" << endl;

        // Remove mutex lock on pipe
        pthread_mutex_unlock(perProto->ipPipeSend.pipe_mutex);
    }
}

void* procPerProto::udpIn(void* arg){
    procPerProto* perProto = (procPerProto*) arg;
    
    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Mutext unlock on read pipe
        pthread_mutex_unlock(perProto->udpPipeIn.pipe_mutex);

        // Read from recv pipe
        read(perProto->udpPipeIn.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Mutext lock on read pipe to prevent race conditions
        pthread_mutex_lock(perProto->udpPipeIn.pipe_mutex);

        // Strip headers
        msg = seePipe->msg;
        UDP_header* stripped = (UDP_header*)msg->msgStripHdr(sizeof(UDP_header));
        int protocol_id = stripped->hlp;

        // Strip headers
        //msg = seePipe->msg;
        //int protocol_id = atoi(msg->msgStripHdr(2));
        //msg->msgStripHdr(6);

        // Create new pipe unit
        pipeStuff outPipe;
        outPipe.protocol_id = 4;
        outPipe.msg = msg;

        if (protocol_id == 7){
            // Lock mutex
            pthread_mutex_lock(perProto->rdpPipeIn.pipe_mutex);

            // Write to pipe
            write(perProto->rdpPipeIn.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

            // Unlock mutex
            pthread_mutex_unlock(perProto->rdpPipeIn.pipe_mutex);
        }
        else if (protocol_id == 8){
            // Lock mutex
            pthread_mutex_lock(perProto->dnsPipeIn.pipe_mutex);

            // Write to pipe
            write(perProto->dnsPipeIn.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

            // Unlock mutex
            pthread_mutex_unlock(perProto->dnsPipeIn.pipe_mutex);
        }
        else{
            cout << "Error invalid protocol going up from udp" << endl;
        }
    }
}

void* procPerProto::ftpSend(void* arg){
    procPerProto* perProto = (procPerProto*) arg;

    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Mutext unlock on read pipe
        pthread_mutex_unlock(perProto->ftpPipeSend.pipe_mutex);

        // Wait for message to send
        read(perProto->ftpPipeSend.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Mutext lock on read pipe to prevent race conditions
        pthread_mutex_lock(perProto->ftpPipeSend.pipe_mutex);

        // Store message in variable
        msg = seePipe->msg;

        // Create new header
        ftpHeader* h = (ftpHeader *) malloc( sizeof(ftpHeader));
        h->m_size = msg->msgLen();
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(ftpHeader));

        // Build new pipe unit
        pipeStuff outPipe;
        outPipe.protocol_id = 5;
        outPipe.msg = msg;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(perProto->tcpPipeSend.pipe_mutex);

        // Write to eth send pipe
        write(perProto->tcpPipeSend.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(perProto->tcpPipeSend.pipe_mutex);
    }
}

void* procPerProto::ftpIn(void* arg){
    procPerProto* perProto = (procPerProto*) arg;
    
    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Mutext unlock on read pipe
        pthread_mutex_unlock(perProto->ftpPipeIn.pipe_mutex);

        // Read from recv pipe
        read(perProto->ftpPipeIn.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Mutext lock on read pipe to prevent race conditions
        pthread_mutex_lock(perProto->ftpPipeIn.pipe_mutex);

        // Strip headers
        msg = seePipe->msg;
        msg->msgStripHdr(sizeof(ftpHeader));

        // Copy message to buffer and terminate line
        char* msg_buf = new char[1024];
        msg->msgFlat(msg_buf);
        msg_buf[msg->msgLen()] = '\n';

        // Print to user
        cout << "FTP Message Recevied: " << msg_buf;

        // Clean up
        delete msg_buf;
    }
}

void* procPerProto::telnetSend(void* arg){
        procPerProto* perProto = (procPerProto*) arg;

    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Mutext unlock on read pipe
        pthread_mutex_unlock(perProto->telnetPipeSend.pipe_mutex);

        // Wait for message to send
        read(perProto->telnetPipeSend.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Mutext lock on read pipe to prevent race conditions
        pthread_mutex_lock(perProto->telnetPipeSend.pipe_mutex);

        // Store message in variable
        msg = seePipe->msg;

        // Create new header
        telnetHeader* h = (telnetHeader*) malloc( sizeof(telnetHeader));
        h->m_size = msg->msgLen();
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(telnetHeader));

        // Build new pipe unit
        pipeStuff outPipe;
        outPipe.protocol_id = 6;
        outPipe.msg = msg;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(perProto->tcpPipeSend.pipe_mutex);

        // Write to eth send pipe
        write(perProto->tcpPipeSend.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(perProto->tcpPipeSend.pipe_mutex);
    }
}

void* procPerProto::telnetIn(void* arg){
    procPerProto* perProto = (procPerProto*) arg;
    
    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Mutext unlock on read pipe
        pthread_mutex_unlock(perProto->telnetPipeIn.pipe_mutex);

        // Read from recv pipe
        read(perProto->telnetPipeIn.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Mutext lock on read pipe to prevent race conditions
        pthread_mutex_lock(perProto->telnetPipeIn.pipe_mutex);

        // Strip headers
        msg = seePipe->msg;
        msg->msgStripHdr(sizeof(telnetHeader));

        // Copy message to buffer and terminate line
        char* msg_buf = new char[1024];
        msg->msgFlat(msg_buf);
        msg_buf[msg->msgLen()] = '\n';

        // Print to user
        cout << "Telnet Message Recevied: " << msg_buf;

        // Clean up
        delete msg_buf;
    }
}

void* procPerProto::rdpSend(void* arg){
        procPerProto* perProto = (procPerProto*) arg;

    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Mutext unlock on read pipe
        pthread_mutex_unlock(perProto->rdpPipeSend.pipe_mutex);

        // Wait for message to send
        read(perProto->rdpPipeSend.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Mutext lock on read pipe to prevent race conditions
        pthread_mutex_lock(perProto->rdpPipeSend.pipe_mutex);

        // Store message in variable
        msg = seePipe->msg;

        // Create new header
        RDP_header* h = (RDP_header *) malloc( sizeof(RDP_header));
        h->m_size = msg->msgLen();
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(RDP_header));

        // Build new pipe unit
        pipeStuff outPipe;
        outPipe.protocol_id = 7;
        outPipe.msg = msg;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(perProto->udpPipeSend.pipe_mutex);

        // Write to eth send pipe
        write(perProto->udpPipeSend.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(perProto->udpPipeSend.pipe_mutex);
    }
}

void* procPerProto::rdpIn(void* arg){
    procPerProto* perProto = (procPerProto*) arg;
    
    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Mutext unlock on read pipe
        pthread_mutex_unlock(perProto->rdpPipeIn.pipe_mutex);

        // Read from recv pip
        read(perProto->rdpPipeIn.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Mutext lock on read pipe to prevent race conditions
        pthread_mutex_lock(perProto->rdpPipeIn.pipe_mutex);

        // Strip headers
        msg = seePipe->msg;
        msg->msgStripHdr(sizeof(RDP_header));

        // Copy message to buffer and terminate line
        char* msg_buf = new char[1024];
        msg->msgFlat(msg_buf);
        msg_buf[msg->msgLen()] = '\n';

        // Print to user
        cout << "RDP Message Recevied: " << msg_buf;

        // Clean up
        delete msg_buf;
    }
}

void* procPerProto::dnsSend(void* arg){
        procPerProto* perProto = (procPerProto*) arg;

    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // For testing
        //cout << "Waiting for DNS message to send" << endl;

        // Mutext unlock on read pipe
        pthread_mutex_unlock(perProto->dnsPipeSend.pipe_mutex);

        // Wait for message to send
        read(perProto->dnsPipeSend.pipe_d[0], (char*)seePipe, sizeof(pipeStuff));

        // Mutext lock on read pipe to prevent race conditions
        pthread_mutex_lock(perProto->dnsPipeSend.pipe_mutex);

        // For testing
        //cout << "Protocol ID test: " << seePipe->protocol_id << endl;

        // Store message in variable
        msg = seePipe->msg;

        // For testing
        //char* test = new char[1024];
        //msg->msgFlat(test);
        //cout << "Message received for sending in dnsSend: " << test << endl;

        // Create new header
        DNS_header* h = (DNS_header *) malloc( sizeof(DNS_header));
        h->m_size = msg->msgLen();

        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(DNS_header));
        
        // For testing
        //DNS_header* h2 = (DNS_header*)msg->msgStripHdr(sizeof(DNS_header));
        //cout << "Size after adding DNS_header: " << msg->msgLen() << endl;

        // Build new pipe unit
        pipeStuff outPipe;
        outPipe.protocol_id = 8;
        outPipe.msg = msg;

        // For testing
        //cout << "DNS pipe unit built" << endl;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(perProto->udpPipeSend.pipe_mutex);

        // Write to eth send pipe
        write(perProto->udpPipeSend.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(perProto->udpPipeSend.pipe_mutex);
    }
}

void* procPerProto::dnsIn(void* arg){
    procPerProto* perProto = (procPerProto*) arg;
    
    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Mutext unlock on read pipe
        pthread_mutex_unlock(perProto->dnsPipeIn.pipe_mutex);

        // Read from recv pipe
        read(perProto->dnsPipeIn.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Mutext lock on read pipe to prevent race conditions
        pthread_mutex_lock(perProto->dnsPipeIn.pipe_mutex);

        // Strip headers
        msg = seePipe->msg;
        msg->msgStripHdr(sizeof(DNS_header));

        // Copy message to buffer and terminate line
        char* msg_buf = new char[1024];
        msg->msgFlat(msg_buf);
        msg_buf[msg->msgLen()] = '\n';

        // Print to user
        cout << "DNS Message Recevied: " << msg_buf;

        // Clean up
        delete msg_buf;
    }
}
