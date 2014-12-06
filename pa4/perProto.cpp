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

procPerProto::procPerProto(char out[], char in[]){
    //Start up the communication
    comInit(out, in);


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
    ftpPipeSend.pipeMutex = &ftpMutexSend;
    ftpMutexIn = PTHREAD_MUTEX_INITIALIZER;
    ftpPipeIn.pipeMutex = &ftpMutexIn;

    telnetMutexSend = PTHREAD_MUTEX_INITIALIZER;
    telnetPipeSend.pipeMutex = &telnetMutexSend;
    telnetMutexIn = PTHREAD_MUTEX_INITIALIZER;
    telnetPipeIn.pipeMutex = &telnetMutexIn;

    rdpMutexSend = PTHREAD_MUTEX_INITIALIZER;
    rdpPipeSend.pipeMutex = &rdpMutexSend;
    rdpMutexIn = PTHREAD_MUTEX_INITIALIZER;
    rdpPipeIn.pipeMutex = &rdpMutexIn;

    dnsMutexSend = PTHREAD_MUTEX_INITIALIZER;
    dnsPipeSend.pipeMutex = &dnsMutexSend;
    dnsMutexIn = PTHREAD_MUTEX_INITIALIZER;
    dnsPipeIn.pipeMutex = &dnsMutexIn;

    tcpMutexSend = PTHREAD_MUTEX_INITIALIZER;
    tcpPipeSend.pipeMutex = &tcpMutexSend;
    tcpMutexIn = PTHREAD_MUTEX_INITIALIZER;
    tcpPipeIn.pipeMutex = &tcpMutexIn;

    udpMutexSend = PTHREAD_MUTEX_INITIALIZER;
    udpPipeSend.pipeMutex = &udpMutexSend;
    udpMutexIn = PTHREAD_MUTEX_INITIALIZER;
    udpPipeIn.pipeMutex = &udpMutexIn;

    ipMutexSend = PTHREAD_MUTEX_INITIALIZER;
    ipPipeSend.pipeMutex = &ipMutexSend;
    ipMutexIn = PTHREAD_MUTEX_INITIALIZER;
    ipPipeIn.pipeMutex = &ipMutexIn;

    ethMutexSend = PTHREAD_MUTEX_INITIALIZER;
    ethPipeSend.pipeMutex = &ethMutexSend;
    ethMutexIn = PTHREAD_MUTEX_INITIALIZER;
    ethPipeIn.pipeMutex = &ethMutexIn;

    threadPool = new ThreadPool(16);

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
    delete threadPool;
}

void procPerProto::comInit(char out[], char in[]){
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
        pthread_mutex_lock(perProto->ethPipeIn.pipeMutex);

        // Write to ethernet in pipe
        write(perProto->ethPipeIn.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

        //Unlock
        pthread_mutex_unlock(perProto->ethPipeIn.pipeMutex);
    }
}

void procPerProto::messageSend(Message* msg, int protocol_id){
    //Is it coming through FTP?
    if (protocol_id == 5) {
        pipeStuff ftp;
        ftp.protocol_id = 5;
        ftp.msg = msg;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(ftpPipeSend.pipeMutex);

        // Write to ftp send pipe
        write(ftpPipeSend.pipe_d[1], (char*) &ftp, sizeof(pipeStuff));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(ftpPipeSend.pipeMutex);
    }

    else if (protocol_id == 6){
        pipeStuff tel;
        tel.protocol_id = 6;
        tel.msg = msg;
        
        // Acquire mutex lock on pipe
        pthread_mutex_lock(telnetPipeSend.pipeMutex);

        // Write to ftp send pipe
        write(telnetPipeSend.pipe_d[1], (char*) &tel, sizeof(pipeStuff));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(telnetPipeSend.pipeMutex);
    }

    else if (protocol_id == 7){
        pipeStuff rdp;
        rdp.protocol_id = 7;
        rdp.msg = msg;
        
        // Lock pipe
        pthread_mutex_lock(rdpPipeSend.pipeMutex);

        // Write to pipe
        write(rdpPipeSend.pipe_d[1], (char*) &rdp, sizeof(pipeStuff));

        // Unlock pipe
        pthread_mutex_unlock(rdpPipeSend.pipeMutex);
    }

    else if (protocol_id == 8){
        pipeStuff dns;
        dns.protocol_id = 8;
        dns.msg = msg;

        // Lock pipe
        pthread_mutex_lock(dnsPipeSend.pipeMutex);

        // Write to pipe
        write(dnsPipeSend.pipe_d[1], (char*) &dns, sizeof(pipeStuff));

        // Unlock Pipe
        pthread_mutex_unlock(dnsPipeSend.pipeMutex);
    }

    else{
        cout << "Invalid protocol_id" << endl;
    }
}

void* procPerProto::ethSend(void* arg){
    procPerProto* perProto = (procPerProto*) arg;
    struct sockaddr_in serverAddress;
    socklen_t len;
    struct hostent *phe;   

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;

    char* host = "localhost";
   
    // Host to IP
    if ( (phe = gethostbyname(host)) )
    {
        memcpy(&serverAddress.sin_addr, phe->h_addr, phe->h_length);
    }
    else if ( (serverAddress.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
        errexit("ERROR in  \"%s\" host entry\n", host);

    // String to int
    if ((serverAddress.sin_port = htons((unsigned short)atoi(perProto->mutSendPort))) == 0)
        errexit("ERROR on  \"%s\" port number\n", perProto->mutSendPort);



    // Makin sockets
    int udpSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSock < 0)
        errexit("ERROR: Cannot create socket: %s\n", strerror(errno));
    
    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Unlock pipe
        pthread_mutex_unlock(perProto->ethPipeSend.pipeMutex);

        // Hang on read
        read(perProto->ethPipeSend.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Lock pip
        pthread_mutex_lock(perProto->ethPipeSend.pipeMutex);

        // Store message
        msg = seePipe->msg;

        // Create header
        ethHeader* h = (ethHeader *) malloc( sizeof(ethHeader));
        h->hlp = seePipe->protocol_id;
        h->m_size = msg->msgLen();
        
        // Add header
        msg->msgAddHdr((char*) h, sizeof(ethHeader));

        char msg_buf[1024];
        memset(&msg_buf, 0, sizeof(msg_buf));
        msg->msgFlat(msg_buf);

        // Start on the chain
        if (sendto(udpSock, msg_buf, msg->msgLen(), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
        printf("Error with sendto %s\n", strerror(errno));

    }


}

void* procPerProto::ethIn(void* arg){
    procPerProto* perProto = (procPerProto*) arg;
    
    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Unlock pipe
        pthread_mutex_unlock(perProto->ethPipeIn.pipeMutex);

        // Hang on read
        read(perProto->ethPipeIn.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Lock pipe
        pthread_mutex_lock(perProto->ethPipeIn.pipeMutex);

        // Strip header
        msg = seePipe->msg;
        ethHeader* stripped = (ethHeader*)msg->msgStripHdr(sizeof(ethHeader));
        int protocol_id = stripped->hlp;

        // Create new pipe to do some sending
        pipeStuff outPipe;
        outPipe.protocol_id = 1;
        outPipe.msg = msg;

        if (protocol_id == 2){
            // Lock mutex
            pthread_mutex_lock(perProto->ipPipeIn.pipeMutex);

            // Write to pipe
            write(perProto->ipPipeIn.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

            // Unlock mutex
            pthread_mutex_unlock(perProto->ipPipeIn.pipeMutex);
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

        // Unlock pipe
        pthread_mutex_unlock(perProto->ipPipeSend.pipeMutex);

        // Hang on send
        read(perProto->ipPipeSend.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Lock pipe
        pthread_mutex_lock(perProto->ipPipeSend.pipeMutex);

        // Store message
        msg = seePipe->msg;

        // Create new header
        ipHeader* h = (ipHeader *) malloc( sizeof(ipHeader));
        h->hlp = seePipe->protocol_id;
        h->m_size = msg->msgLen();
        
        // Add header
        msg->msgAddHdr((char*) h, sizeof(ipHeader));

        // Build up pipe
        pipeStuff outPipe;
        outPipe.protocol_id = 2;
        outPipe.msg = msg;

        // Lock pipe
        pthread_mutex_lock(perProto->ethPipeSend.pipeMutex);

        // Write to pipe
        write(perProto->ethPipeSend.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

        // Unlock pipe
        pthread_mutex_unlock(perProto->ethPipeSend.pipeMutex);
    }
}

void* procPerProto::ipIn(void* arg){
    procPerProto* perProto = (procPerProto*) arg;
    
    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Unlock pipe
        pthread_mutex_unlock(perProto->ipPipeIn.pipeMutex);

        // Read pipe
        read(perProto->ipPipeIn.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Lock pipe
        pthread_mutex_lock(perProto->ipPipeIn.pipeMutex);

        // Add header
        msg = seePipe->msg;
        ipHeader* stripped = (ipHeader*)msg->msgStripHdr(sizeof(ipHeader));
        int protocol_id = stripped->hlp;

        // Build up pipe
        pipeStuff outPipe;
        outPipe.protocol_id = 2;
        outPipe.msg = msg;

        if (protocol_id == 3){
            // Lock pipe
            pthread_mutex_lock(perProto->tcpPipeIn.pipeMutex);

            // Write to pipe
            write(perProto->tcpPipeIn.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

            // Unlock pipe
            pthread_mutex_unlock(perProto->tcpPipeIn.pipeMutex);
        }
        else if (protocol_id == 4){
            // Lock pipe
            pthread_mutex_lock(perProto->udpPipeIn.pipeMutex);

            // Write to pipe
            write(perProto->udpPipeIn.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

            // Unlock pipe
            pthread_mutex_unlock(perProto->udpPipeIn.pipeMutex);
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

        // Unlock pipe
        pthread_mutex_unlock(perProto->tcpPipeSend.pipeMutex);

        // Hang on pipe
        read(perProto->tcpPipeSend.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Lock pipe
        pthread_mutex_lock(perProto->tcpPipeSend.pipeMutex);

        // Store message
        msg = seePipe->msg;

        // Make new header
        tcpHeader* h = (tcpHeader *) malloc( sizeof(tcpHeader));
        h->hlp = seePipe->protocol_id;
        h->m_size = msg->msgLen();
        
        // Add header
        msg->msgAddHdr((char*) h, sizeof(tcpHeader));

        // Build up pipe
        pipeStuff outPipe;
        outPipe.protocol_id = 3;
        outPipe.msg = msg;

        // Lock pipe
        pthread_mutex_lock(perProto->ipPipeSend.pipeMutex);

        // Write to pipe
        write(perProto->ipPipeSend.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

        // Lock to pipe
        pthread_mutex_unlock(perProto->ipPipeSend.pipeMutex);
    }
}

void* procPerProto::tcpIn(void* arg){
    procPerProto* perProto = (procPerProto*) arg;
    
    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Unlock pipe
        pthread_mutex_unlock(perProto->tcpPipeIn.pipeMutex);

        // Read pipe
        read(perProto->tcpPipeIn.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Lock pipe
        pthread_mutex_lock(perProto->tcpPipeIn.pipeMutex);

        // Strip headers
        msg = seePipe->msg;
        tcpHeader* stripped = (tcpHeader*)msg->msgStripHdr(sizeof(tcpHeader));
        int protocol_id = stripped->hlp;

        // Create pipe
        pipeStuff outPipe;
        outPipe.protocol_id = 3;
        outPipe.msg = msg;

        if (protocol_id == 5){
            // Lock pipe
            pthread_mutex_lock(perProto->ftpPipeIn.pipeMutex);

            // Write to pipe
            write(perProto->ftpPipeIn.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

            // Unlock pipe
            pthread_mutex_unlock(perProto->ftpPipeIn.pipeMutex);
        }
        else if (protocol_id == 6){
            // Lock pipe
            pthread_mutex_lock(perProto->telnetPipeIn.pipeMutex);

            // Write to pipe
            write(perProto->telnetPipeIn.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

            // Unlock pipe
            pthread_mutex_unlock(perProto->telnetPipeIn.pipeMutex);
        }
        else{
            cout << "Error invalid protocol going up from tcp" << endl;
        }
    }
}

void* procPerProto::udpSend(void* arg){
    procPerProto* perProto = (procPerProto*) arg;

    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Unlock pipe
        pthread_mutex_unlock(perProto->udpPipeSend.pipeMutex);

        // Hang on pipe
        read(perProto->udpPipeSend.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Lock pipe
        pthread_mutex_lock(perProto->udpPipeSend.pipeMutex);

        // Save message
        msg = seePipe->msg;

        // Create header
        udpHeader* h = (udpHeader *) malloc( sizeof(udpHeader));
        h->hlp = seePipe->protocol_id;
        h->m_size = msg->msgLen();
        
        // Add header
        msg->msgAddHdr((char*) h, sizeof(udpHeader));

        // Build up pipe
        pipeStuff outPipe;
        outPipe.protocol_id = 4;
        outPipe.msg = msg;

        // Lock pipe
        pthread_mutex_lock(perProto->ipPipeSend.pipeMutex);

        // Write to pipe
        write(perProto->ipPipeSend.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

        // Unlock pipe
        pthread_mutex_unlock(perProto->ipPipeSend.pipeMutex);
    }
}

void* procPerProto::udpIn(void* arg){
    procPerProto* perProto = (procPerProto*) arg;
    
    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Unlock pipe
        pthread_mutex_unlock(perProto->udpPipeIn.pipeMutex);

        // Read pipe
        read(perProto->udpPipeIn.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Lock pipe
        pthread_mutex_lock(perProto->udpPipeIn.pipeMutex);

        // Strip headers
        msg = seePipe->msg;
        udpHeader* stripped = (udpHeader*)msg->msgStripHdr(sizeof(udpHeader));
        int protocol_id = stripped->hlp;

        // Create new pipe
        pipeStuff outPipe;
        outPipe.protocol_id = 4;
        outPipe.msg = msg;

        if (protocol_id == 7){
            // Lock pipe
            pthread_mutex_lock(perProto->rdpPipeIn.pipeMutex);

            // Write to pipe
            write(perProto->rdpPipeIn.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

            // Unlock pipe
            pthread_mutex_unlock(perProto->rdpPipeIn.pipeMutex);
        }
        else if (protocol_id == 8){
            // Lock pipe
            pthread_mutex_lock(perProto->dnsPipeIn.pipeMutex);

            // Write to pipe
            write(perProto->dnsPipeIn.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

            // Unlock pipe
            pthread_mutex_unlock(perProto->dnsPipeIn.pipeMutex);
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

        // Unlock pipe
        pthread_mutex_unlock(perProto->ftpPipeSend.pipeMutex);

        // Hang on pipe
        read(perProto->ftpPipeSend.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Lock pipe
        pthread_mutex_lock(perProto->ftpPipeSend.pipeMutex);

        // Save
        msg = seePipe->msg;

        // Create header
        ftpHeader* h = (ftpHeader *) malloc( sizeof(ftpHeader));
        h->m_size = msg->msgLen();
        
        // Add header
        msg->msgAddHdr((char*) h, sizeof(ftpHeader));

        // Build up pipe
        pipeStuff outPipe;
        outPipe.protocol_id = 5;
        outPipe.msg = msg;

        // Lock pipe
        pthread_mutex_lock(perProto->tcpPipeSend.pipeMutex);

        // Write to pipe
        write(perProto->tcpPipeSend.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

        // Unlock pipe
        pthread_mutex_unlock(perProto->tcpPipeSend.pipeMutex);
    }
}

void* procPerProto::ftpIn(void* arg){
    procPerProto* perProto = (procPerProto*) arg;
    
    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Unlock pipe
        pthread_mutex_unlock(perProto->ftpPipeIn.pipeMutex);

        // Read pipe
        read(perProto->ftpPipeIn.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Lock pipe
        pthread_mutex_lock(perProto->ftpPipeIn.pipeMutex);

        // Strip headers
        msg = seePipe->msg;
        msg->msgStripHdr(sizeof(ftpHeader));

        // Message copy
        char* msg_buf = new char[1024];
        msg->msgFlat(msg_buf);
        msg_buf[msg->msgLen()] = '\n';

        // Output
        cout << "FTP Message Recevied: " << msg_buf;

        // Destroy
        delete msg_buf;
    }
}

void* procPerProto::telnetSend(void* arg){
        procPerProto* perProto = (procPerProto*) arg;

    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Unlock pipe
        pthread_mutex_unlock(perProto->telnetPipeSend.pipeMutex);

        // Hang on pipe
        read(perProto->telnetPipeSend.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Lock pipe
        pthread_mutex_lock(perProto->telnetPipeSend.pipeMutex);

        // Save message
        msg = seePipe->msg;

        // Create header
        telnetHeader* h = (telnetHeader*) malloc( sizeof(telnetHeader));
        h->m_size = msg->msgLen();
        
        // Add header
        msg->msgAddHdr((char*) h, sizeof(telnetHeader));

        // Build up pipe
        pipeStuff outPipe;
        outPipe.protocol_id = 6;
        outPipe.msg = msg;

        // Lock pipe
        pthread_mutex_lock(perProto->tcpPipeSend.pipeMutex);

        // Write to pipe
        write(perProto->tcpPipeSend.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

        // Unlock pipe
        pthread_mutex_unlock(perProto->tcpPipeSend.pipeMutex);
    }
}

void* procPerProto::telnetIn(void* arg){
    procPerProto* perProto = (procPerProto*) arg;
    
    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Unlock pipe
        pthread_mutex_unlock(perProto->telnetPipeIn.pipeMutex);

        // Read pipe
        read(perProto->telnetPipeIn.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Lock pipe
        pthread_mutex_lock(perProto->telnetPipeIn.pipeMutex);

        // Strip headers
        msg = seePipe->msg;
        msg->msgStripHdr(sizeof(telnetHeader));

        // Message copy
        char* msg_buf = new char[1024];
        msg->msgFlat(msg_buf);
        msg_buf[msg->msgLen()] = '\n';

        // OUtput
        cout << "Telnet Message Recevied: " << msg_buf;

        // Destroy
        delete msg_buf;
    }
}

void* procPerProto::rdpSend(void* arg){
        procPerProto* perProto = (procPerProto*) arg;

    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Unlock pipe
        pthread_mutex_unlock(perProto->rdpPipeSend.pipeMutex);

        // Hang on pipe
        read(perProto->rdpPipeSend.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Lock pipe
        pthread_mutex_lock(perProto->rdpPipeSend.pipeMutex);

        // Save
        msg = seePipe->msg;

        // Create header
        rdpHeader* h = (rdpHeader *) malloc( sizeof(rdpHeader));
        h->m_size = msg->msgLen();
        
        // Add header
        msg->msgAddHdr((char*) h, sizeof(rdpHeader));

        // Build up pipe
        pipeStuff outPipe;
        outPipe.protocol_id = 7;
        outPipe.msg = msg;

        // Lock pipe
        pthread_mutex_lock(perProto->udpPipeSend.pipeMutex);

        // Write to pipe
        write(perProto->udpPipeSend.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

        // Unlock pipe
        pthread_mutex_unlock(perProto->udpPipeSend.pipeMutex);
    }
}

void* procPerProto::rdpIn(void* arg){
    procPerProto* perProto = (procPerProto*) arg;
    
    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Unlock pipe
        pthread_mutex_unlock(perProto->rdpPipeIn.pipeMutex);

        // Read pipe
        read(perProto->rdpPipeIn.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Lock pipe
        pthread_mutex_lock(perProto->rdpPipeIn.pipeMutex);

        // Strip headers
        msg = seePipe->msg;
        msg->msgStripHdr(sizeof(rdpHeader));

        // Message Copy
        char* msg_buf = new char[1024];
        msg->msgFlat(msg_buf);
        msg_buf[msg->msgLen()] = '\n';

        // Output
        cout << "RDP Message Recevied: " << msg_buf;

        // Destroy
        delete msg_buf;
    }
}

void* procPerProto::dnsSend(void* arg){
        procPerProto* perProto = (procPerProto*) arg;

    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Unlock pipe
        pthread_mutex_unlock(perProto->dnsPipeSend.pipeMutex);

        // Hang on pipe
        read(perProto->dnsPipeSend.pipe_d[0], (char*)seePipe, sizeof(pipeStuff));

        //Lock pipes
        pthread_mutex_lock(perProto->dnsPipeSend.pipeMutex);

        // Save
        msg = seePipe->msg;

        // Create header
        dnsHeader* h = (dnsHeader *) malloc( sizeof(dnsHeader));
        h->m_size = msg->msgLen();

        // Add header
        msg->msgAddHdr((char*) h, sizeof(dnsHeader));

        // Build up pipe
        pipeStuff outPipe;
        outPipe.protocol_id = 8;
        outPipe.msg = msg;

        // Lock pipe
        pthread_mutex_lock(perProto->udpPipeSend.pipeMutex);

        // Write to pipe
        write(perProto->udpPipeSend.pipe_d[1], (char*) &outPipe, sizeof(pipeStuff));

        // Unlock pipe
        pthread_mutex_unlock(perProto->udpPipeSend.pipeMutex);
    }
}

void* procPerProto::dnsIn(void* arg){
    procPerProto* perProto = (procPerProto*) arg;
    
    while(1){
        Message* msg;
        pipeStuff* seePipe = new pipeStuff;

        // Unlock pipe
        pthread_mutex_unlock(perProto->dnsPipeIn.pipeMutex);

        // Read from pipe
        read(perProto->dnsPipeIn.pipe_d[0], (char*) seePipe, sizeof(pipeStuff));

        // Lock pipe
        pthread_mutex_lock(perProto->dnsPipeIn.pipeMutex);

        // Strip headers
        msg = seePipe->msg;
        msg->msgStripHdr(sizeof(dnsHeader));

        // Message Copy
        char* msg_buf = new char[1024];
        msg->msgFlat(msg_buf);
        msg_buf[msg->msgLen()] = '\n';

        // OUtput
        cout << "DNS Message Recevied: " << msg_buf;

        // Destroy
        delete msg_buf;
    }
}
