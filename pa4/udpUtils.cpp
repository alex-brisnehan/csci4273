#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <sys/errno.h>
#include <stdarg.h>

using namespace std;

// print an error message and exit
int errexit(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(1);
}

int udpSocket(char portnum[])
/*
 * Arguments:
 *      portnum - port number of the server
 */
{
    struct sockaddr_in sin; /* an Internet endpoint address  */
    int    s;               /* socket descriptor             */

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;

    /* Map port number (char string) to port number (int) */
    if ((sin.sin_port = htons((unsigned short)atoi(portnum))) == 0)
        errexit("can't get \"%s\" port number\n", portnum);

    /* Allocate a socket */
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0)
        errexit("can't create socket: %s\n", strerror(errno));

    /* Bind the socket */
    if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        fprintf(stderr, "can't bind to default port %s: %s; Trying other port\n", portnum, strerror(errno));
        sin.sin_port = htons(0); /* request a port number to be allocated by bind */
        if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
            errexit("can't bind: %s\n", strerror(errno));
        else
        {
            socklen_t socklen = sizeof(sin);

            if (getsockname(s, (struct sockaddr *)&sin, &socklen) < 0)
                errexit("getsockname: %s\n", strerror(errno));
        }
        std::string new_port = std::to_string(ntohs(sin.sin_port));
        int i = 0;
        for (std::string::iterator it = new_port.begin(); it != new_port.end(); it++)
        {
            printf("%c\n", *it);
            portnum[i++] = *it;
        }
        portnum[i] = '\0';
    }
    printf("Server running on port: %d\n", ntohs(sin.sin_port));
    return s;
}
