/* Name: Alexander Brisnehan
 * File: echoClient
 * Date: 10/10/14
 * Uses code from existing echoClient given to us
 * Use: ./echoClient [port]
*/





#include <string>
#include <sys/types.h>
#include <sys/socket.h>

#include <sys/errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <openssl/ssl.h>

using namespace std;
#ifndef INADDR_NONE
#define INADDR_NONE     0xffffffff
#endif  /* INADDR_NONE */

extern int	errno;

int	TCPecho(const char *host, const char *portnum);
int	errexit(const char *format, ...);
int	connectsock(const char *host, const char *portnum);

#define	LINELEN		128

/*------------------------------------------------------------------------
 * main - TCP client for ECHO service
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	char	*host = "localhost";	/* host to use if none supplied	*/
	char	*portnum = "5004";	/* default server port number	*/

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
int TCPecho(const char *host, const char *portnum)
{
	char	buf1[LINELEN+1];		/* buffer for one line of text	*/
	char	buf2[LINELEN+1];
	int	s, n;			/* socket descriptor, read count*/
	int	outchars, inchars;	/* characters sent and received	*/

	string passwdString;
	char passwd[LINELEN+1];

	//The Certificates
	char *caCert = "cacert.pem";
	char *clPrivate = "cakey.pem";

	//The SSL stuff
	SSL *ssl;
	SSL_CTX *ctx;

	//Initialize the SSL
	SSL_library_init();
	SSL_load_error_strings();

	const SSL_METHOD *method = SSLv3_client_method();
	ctx = SSL_CTX_new(method);

	//Ask for the password
	printf("Enter the password to decrypt the key\n");

	//Hide the input in the terminal for the password
	termios oldt;
	tcgetattr(STDIN_FILENO, &oldt);
	termios newt = oldt;
	newt.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	//Get password
	fgets(passwd, sizeof(passwd), stdin);

	//Restore terminal 
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	//Create the password string
	passwdString = "";
	for(int i = 0; i<= LINELEN; i++){
	    if(passwd[i] != '\0' && passwd[i] != ' ' && passwd[i] != '\n'){
		passwdString += passwd[i];
	    }
	    else{
		break;
	    }
	}
	passwdString += '\0';
	char *str =  strncpy(buf1, passwdString.c_str(), sizeof(buf1));

	//Do SSL password mumbo jumbo
	SSL_CTX_set_default_passwd_cb_userdata(ctx, str);

	//Grab them certs by comparing the password
	if(SSL_CTX_load_verify_locations(ctx,caCert,NULL) != 1){
	    printf("Can't get cert file. Make sure the password is correct\n");
	    exit(0);
	}
	if(SSL_CTX_use_PrivateKey_file(ctx,clPrivate,SSL_FILETYPE_PEM) != 1){
	    printf("Can't get privatekey file. Make sure the password is correct\n");
	    exit(0);
	}
	
	//Start up echo
	printf("Start talking!\n");

	//Lets start securing stuff
	SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);

	if(ctx == NULL){
	    exit(0);
	}

	ssl = SSL_new(ctx);

	//Its a go for a secure socket

	s = connectsock(host, portnum);
	SSL_set_fd(ssl, s);

	if(SSL_connect(ssl) < 1){
	    errexit("Failure on socket read: %s\n", strerror(errno));
	    exit(1);
	}

	while (1) {
		fgets(buf1, sizeof(buf1), stdin);
		buf1[LINELEN] = '\0';	/* insure line null-terminated	*/
		//Secure echo
		SSL_write(ssl, buf1, sizeof(buf1));
		memset(&buf1, 0, sizeof(buf1));
		n = SSL_read(ssl, &buf2, sizeof(buf2));

		if(n < 0){
			    errexit("Failure on socket read: %s\n", strerror(errno));
		}
		fputs(buf2, stdout);
		memset(&buf2, 0, sizeof(buf2));
	}
}

/*------------------------------------------------------------------------
 * errexit - print an error message and exit
 *------------------------------------------------------------------------
 */
int errexit(const char *format, ...)
{
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
int connectsock(const char *host, const char *portnum)
/*
 * Arguments:
 *      host      - name of host to which connection is desired
 *      portnum   - server port number
 */
{
        struct hostent  *phe;   /* pointer to host information entry    */
        struct sockaddr_in sin; /* an Internet endpoint address         */
        int     s;              /* socket descriptor                    */


        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;

    /* Map port number (char string) to port number (int)*/
        if ((sin.sin_port=htons((unsigned short)atoi(portnum))) == 0)
                errexit("can't get \"%s\" port number\n", portnum);

    /* Map host name to IP address, allowing for dotted decimal */
        if ( phe = gethostbyname(host) )
                memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
        else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
                errexit("can't get \"%s\" host entry\n", host);

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

