#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include "perMessage.h"
#include "protoModel.h"
#include "udpUtils.cpp"

procPerMessage::procPerMessage(char* outPort, char* inPort)
{
    //Create thread for the message to be sent

    threadPool = new ThreadPool(25);
    mutRecvPort = inPort;
    mutSendPort = outPort;

    pthread_t tid;
    int err = pthread_create(&tid, NULL, procPerMessage::seeUdp, (void*) this);
    if (err != 0) {
        cout << "Creating the pthread failed: " << err << endl;
        exit(1);
    }
}

procPerMessage::~procPerMessage()
{
    //Just destructor things
    delete threadPool;
}

void procPerMessage::ethernetIn(void* arg)
{
    //The bread and butter : The message coming in
    Message* msg = (Message*) arg;

    //Bye bye header
    ethHeader* stripped = (ethHeader*)msg->msgStripHdr(sizeof(ethHeader));

    //What is the id of the previous place
    int protocol_id = stripped->hlp;

    //Oh thats right, its IP
    if (protocol_id == IP_ID)
        ipIn(msg);
    else
        printf("ERROR: Something wrong with protocol id %d in ethernetIn\n", protocol_id);
}

void procPerMessage::ethernetSend(int protocol_id, Message* msg)
{
    //Initialize all that good stuff
    ethHeader *h = new ethHeader;
    char* msg_buf = new char[1024];
    socklen_t len;
    struct sockaddr_in serverAddress;
    struct hostent *phe;
    h->hlp = protocol_id;
    h->m_size = msg->msgLen();

    //Prep it for sending
    msg->msgAddHdr((char*) h, sizeof(ethHeader));
    msg->msgFlat(msg_buf);

    //Which is client, which is server
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;

    char* host = "localhost";
    
    // hostname to address
    if ( (phe = gethostbyname(host)) )
    {
        memcpy(&serverAddress.sin_addr, phe->h_addr, phe->h_length);
    }
    else if ( (serverAddress.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
        errexit("ERROR: Something in \"%s\" host entry\n", host);

    // String to int
    if ((serverAddress.sin_port = htons((unsigned short)atoi(mutSendPort))) == 0)
        errexit("ERROR: Something in \"%s\" port number\n", mutSendPort);

    //Makin socket
    int udpSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSock < 0)
        errexit("ERROR: create socket: %s\n", strerror(errno));

    if (sendto(udpSock, msg_buf, msg->msgLen(), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
        printf("Error with socket %s\n", strerror(errno));

    delete h;
    delete msg;
    delete[] msg_buf;
}

void procPerMessage::ipIn(Message* msg)
{

    //Get rid o the header
    ipHeader* stripped = (ipHeader*)msg->msgStripHdr(sizeof(ipHeader));
    int protocol_id = stripped->hlp;

    //Check where it is going. TCP to tcp; UDP to udp
    if (protocol_id == TCP_ID)
        tcpIn(msg);
    else if (protocol_id == UDP_ID)
        udpIn(msg);
    else
        printf("ERROR: Something with protocol id %d in ipIn\n", protocol_id);
}

void procPerMessage::ipSend(int protocol_id, Message* msg)
{

    //Add header 
    ipHeader *header = new ipHeader;
    header->hlp = protocol_id;

    //Coming back, send to ethernet instead
    header->m_size = msg->msgLen();

    msg->msgAddHdr((char *)header, sizeof(ipHeader));
    ethernetSend(IP_ID, msg);

    delete header;
}

void procPerMessage::tcpIn(Message* msg)
{

    //Get rid of header
    tcpHeader* stripped = (tcpHeader*)msg->msgStripHdr(sizeof(tcpHeader));
    int protocol_id = stripped->hlp;

    //Check where it is going
    if (protocol_id == FTP_ID)
        ftpIn(msg);
    else if (protocol_id == TELNET_ID)
        telnetIn(msg);
    else
        printf("ERROR: Something went wrong with protocol id %d in tcpIn\n", protocol_id);
}

void procPerMessage::tcpSend(int protocol_id, Message* msg)
{
    //Add header
    tcpHeader *header = new tcpHeader;
    header->hlp = protocol_id;

    //Send to ip
    header->m_size = msg->msgLen();

    msg->msgAddHdr((char *)header, sizeof(tcpHeader));
    ipSend(TCP_ID, msg);

    delete header;
}

void procPerMessage::udpIn(Message* msg)
{
    //Get rid of header
    udpHeader* stripped = (udpHeader*)msg->msgStripHdr(sizeof(udpHeader));
    int protocol_id = stripped->hlp;

    //Check where it goes
    if (protocol_id == RDP_ID)
        rdpIn(msg);
    else if (protocol_id == DNS_ID)
        dnsIn(msg);
    else
        printf("ERROR: Something went wrong with protocol id %d in udpIn\n", protocol_id);
}

void procPerMessage::udpSend(int protocol_id, Message* msg)
{
    //Add header
    udpHeader *header = new udpHeader;
    header->hlp = protocol_id;

    //Send to IP
    header->m_size = msg->msgLen();

    msg->msgAddHdr((char *)header, sizeof(udpHeader));
    ipSend(UDP_ID, msg);

    delete header;
}

void* procPerMessage::seeUdp(void* arg)
{
    procPerMessage* perMessage = (procPerMessage*) arg;
    char* udpPortNumber = perMessage->mutRecvPort;
    int udpSock = udpSocket(udpPortNumber);

    struct sockaddr_in clientAddress;
    socklen_t len;
    int mess;

    while (1) {
        char* msg_buf = new char[1024];
        memset(msg_buf, 0, 1024);

        len = sizeof(clientAddress);
        mess = recvfrom(udpSock, msg_buf, 1024, 0, (struct sockaddr *)&clientAddress, &len);
        Message* msg = new Message(msg_buf, mess);
        perMessage->threadPool->dispatch_thread(procPerMessage::ethernetIn, (void*) msg);
        // delete[] msg_buf;
    }
}

void procPerMessage::ftpIn(Message* msg)
{
    //Strip header and print
    char buf[1024];
    ftpHeader* stripped = (ftpHeader*)msg->msgStripHdr(sizeof(ftpHeader));
    msg->msgFlat(buf);
    printf("FTP recieved message: %s\n", buf);
    delete msg;
}

void procPerMessage::ftpSend(int protocol_id, Message* msg)
{ 
    //Add header to send out
    ftpHeader *header = new ftpHeader;
    header->hlp = protocol_id;
    header->m_size = msg->msgLen();

    msg->msgAddHdr((char *)header, sizeof(ftpHeader));
    tcpSend(FTP_ID, msg);

    delete header;
}

void procPerMessage::telnetIn(Message* msg)
{
    //Strip header and print
    char buf[1024];
    telnetHeader* stripped = (telnetHeader*)msg->msgStripHdr(sizeof(telnetHeader));
    msg->msgFlat(buf);
    printf("telnet recieved message %s\n", buf);
    delete msg;
}

void procPerMessage::telnetSend(int protocol_id, Message* msg)
{
    //Add header and send
    telnetHeader *header = new telnetHeader;
    header->hlp = protocol_id;
    header->m_size = msg->msgLen();

    msg->msgAddHdr((char *)header, sizeof(telnetHeader));
    tcpSend(TELNET_ID, msg);

    delete header;
}

void procPerMessage::rdpIn(Message* msg)
{
    //Strip header and print
    char buf[1024];
    rdpHeader* stripped = (rdpHeader*)msg->msgStripHdr(sizeof(rdpHeader));
    msg->msgFlat(buf);
    printf("RDP recieved message %s\n", buf);
    delete msg;
}

void procPerMessage::rdpSend(int protocol_id, Message* msg)
{
    //Add header and send
    rdpHeader *header = new rdpHeader;
    header->hlp = protocol_id;
    header->m_size = msg->msgLen();

    msg->msgAddHdr((char *)header, sizeof(rdpHeader));
    udpSend(RDP_ID, msg);

    delete header;
}

void procPerMessage::dnsIn(Message* msg)
{
    //Strip header and print
    char buf[1024];
    dnsHeader* stripped = (dnsHeader*)msg->msgStripHdr(sizeof(dnsHeader));
    msg->msgFlat(buf);
    printf("DNS recieved message %s\n", buf);
    delete msg;
}

void procPerMessage::dnsSend(int protocol_id, Message* msg)
{
    //Add header and send
    dnsHeader *header = new dnsHeader;
    header->hlp = protocol_id;
    header->m_size = msg->msgLen();

    msg->msgAddHdr((char *)header, sizeof(dnsHeader));
    udpSend(DNS_ID, msg);

    delete header;
}

