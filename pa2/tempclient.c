SCI 4273 Fall 2014
 *
 * Programming Assignment 2: Echo Client Program with SSl
 *
 * Author: Christopher Jordan
 *
 * Updated: 10/06/2014
 */

#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/errno.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <openssl/ssl.h>

using namespace std;

#ifndef INADDR_NONE
#define INADDR_NONE     0xffffffff
#endif  /* INADDR_NONE */

extern int	errno;
int	    TCPecho(const char *host, const char *portnum);
int	    errexit(const char *format, ...);
int	    connectsock(const char *host, const char *portnum);

#define	LINELEN		128

/*------------------------------------------------------------------------
 * main - TCP client for ECHO service
 *------------------------------------------------------------------------
 */
int main(int argc, char *argv[]){
	char	*host = "localhost";	  /* host to use if none supplied	*/
	char	*portnum = "5004";       /* default server port number	*/

	switch (argc) {
	case 1:
		host = "localhost";
		break;
	case 3:
		host = argv[2];
		/* FALL THROUGH */
	case 2:
		portnum = argv[1];
		break;
	default:
		fprintf(stderr, "usage: TCPecho [host [port]]\n");
		exit(1);
	}
	TCPecho(host, portnum);
	exit(0);
}

/*------------------------------------------------------------------------
 * TCPecho - send input to ECHO service on specified host and print reply
 *------------------------------------------------------------------------
 */
int TCPecho(const char *host, const char *portnum) {
	char buf[LINELEN+1];		/* buffer for one line of text	*/
    char rebuf[LINELEN+1];
	int	s, n;			/* socket descriptor, read count*/
    
    // Password variables
    char pass_in[LINELEN+1];
    string pass_str;

    // File locations for certicate and key
    char *cl_priv = "cakey.pem";
    char *ca_cert = "cacert.pem";

    // SSL Variables
    SSL *ssl;
    SSL_CTX *ctx;

    // Initialize SSL library and crypto suite
    SSL_library_init();
    SSL_load_error_strings();

    // Intialize CTX state
    const SSL_METHOD *method = SSLv3_client_method();
    ctx = SSL_CTX_new(method);

    // Request private key password from user
    printf("Please enter the private key decryption password\n");

    // Read from command line
    fgets(pass_in, sizeof(pass_in), stdin);

    // Convert password to string then store back
    pass_str = "";
    for (int i = 0; i <= LINELEN; i++){
        if ( pass_in[i] != '\0' && pass_in[i] != '\n' && pass_in[i] != ' '){
            pass_str += pass_in[i];
        }
        else{
            break;
        }
    }
    pass_str += '\0';
    char *pass = strncpy(buf, pass_str.c_str(), sizeof(buf));
    
    // Set password callback
    SSL_CTX_set_default_passwd_cb_userdata(ctx, pass);

    // Load the local private key from the location specified by keyFile
    if ( SSL_CTX_use_PrivateKey_file(ctx, cl_priv, SSL_FILETYPE_PEM) != 1 ){
        printf("Unable to load privatekey file\n");
        exit(0);
    }

    // Load the CA certificate for verification
    if (SSL_CTX_load_verify_locations(ctx, ca_cert, NULL) != 1){
        printf("Unable to load ca cert file\n");
        exit(0);
    }

    // Require verification
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);

    // Ensure ctx not null
    if ( ctx == NULL ){
        exit(0);
    }

    // Initialize an ssl connection state
    ssl = SSL_new(ctx);

    // Start a TCP socket
    s = connectsock(host, portnum);

    // Map ssl to socket
    SSL_set_fd(ssl, s);

    // Connect the SSL socket or error out
    if ( SSL_connect(ssl) < 1 ){
        errexit("socket read failed: %s\n", strerror(errno));
        exit(1);
    }

	while (1) {
        // Read from command line
        fgets(buf, sizeof(buf), stdin);

        // Ensure buffer is null terminated
		buf[LINELEN] = '\0';

        // Write the text from the console the server via SSL
        SSL_write(ssl, buf, sizeof(buf));

        // Clear send buffer
        memset(&buf, 0, sizeof(buf));

        // Read and decrypt reply via SSL connection
        n = SSL_read(ssl, &rebuf, sizeof(rebuf));
			
        // If no characters read, then error out
        if (n < 0)
			errexit("socket read failed: %s\n", strerror(errno));

        // Print the echo to the console
		fputs(rebuf, stdout);

        // Clear recieve buffer
        memset(&rebuf, 0, sizeof(rebuf));
	}
}

/*------------------------------------------------------------------------
 * errexit - print an error message and exit
 *------------------------------------------------------------------------
 */
int errexit(const char *format, ...){
        va_list args;

        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
        exit(1);
}

/*------------------------------------------------------------------------
 * connectsock - allocate & connect a socket using TCP 
 *------------------------------------------------------------------------
 */
int connectsock(const char *host, const char *portnum){
/*
 * Arguments:
 *      host      - name of host to which connection is desired
 *      portnum   - server port number
 */
        struct hostent  *phe;   /* pointer to host information entry    */
        struct sockaddr_in sin; /* an Internet endpoint address         */
        int     s;              /* socket descriptor                    */


        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;

    /* Map port number (char string) to port number (int)*/
        if ((sin.sin_port=htons((unsigned short)atoi(portnum))) == 0)
                errexit("can't get \"%s\" port number\n", portnum);

    /* Map host name to IP address, allowing for dotted decimal */
        if ( (phe = gethostbyname(host)) ){
            memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
        }
        else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE ){
            errexit("can't get \"%s\" host entry\n", host);    
        }

    /* Allocate a socket */
        s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (s < 0)
                errexit("can't create socket: %s\n", strerror(errno));

    /* Connect the socket */
        if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
                errexit("can't connect to %s.%s: %s\n", host, portnum,
                        strerror(errno));
        return s;
}

/*------------------------------------------------------------------------
 * pem_passwd_cb - password callback
 *------------------------------------------------------------------------
 */
static int pem_passwd_cb(char *buf, int size, int rwflag, void *password){
  strncpy(buf, (char *)(password), size);
  buf[size - 1] = '\0';
  return(strlen(buf));
}
