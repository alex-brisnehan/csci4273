#ifndef _PROTOCOL_STRUCTS_
#define _PROTOCOL_STRUCTS_

#define ETHERNET_ID 1
#define IP_ID       2
#define TCP_ID      3
#define UDP_ID      4
#define FTP_ID      5
#define TELNET_ID   6
#define RDP_ID      7
#define DNS_ID      8

//Struct to store arguements for the resquester threads
typedef struct {
    // Int for higher level protocol
    int hlp;

    // Char array for other info
    char oi[12];

    // Int for size of message without header
    int m_size;
} ipHeader;

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
} tcpHeader;

//Struct to store arguements for the resquester threads
typedef struct {
    // Int for higher level protocol
    int hlp;

    // Char array for other info
    char oi[4];

    // Int for size of message without header
    int m_size;
} udpHeader;

//Struct to store arguements for the resquester threads
typedef struct {
    // Int for higher level protocol
    int hlp;

    // Char array for other info
    char oi[12];

    // Int for size of message without header
    int m_size;
} rdpHeader;

//Struct to store arguements for the resquester threads
typedef struct {
    // Int for higher level protocol
    int hlp;

    // Char array for other info
    char oi[8];

    // Int for size of message without header
    int m_size;
} dnsHeader;

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

#endif
