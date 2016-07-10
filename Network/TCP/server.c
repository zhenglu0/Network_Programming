/*
    echoserver: TCP/IP sockets example
    get a connect: keep reading data from the socket, echoing
    back the received data.

    usage:    echoserver [-d] [-p port]
*/

#include <stdio.h>
#include <stdlib.h>    /* needed for os x */
#include <string.h>    /* for memset */
#include <sys/socket.h>
#include <arpa/inet.h>    /* for inet_ntoa */
#include <netinet/in.h>
#include <sys/errno.h>   /* defines ERESTART, EINTR */
#include <sys/wait.h>    /* defines WNOHANG, for wait() */
#include <unistd.h>
#include <errno.h>

#include "port.h"       /* defines default port */

#ifndef ERESTART
#define ERESTART EINTR
#endif

#define BUFSIZE 1024

extern int errno;

void serve(int port);    /* main server function */
void disconn(void);
int doprocessing (int rqst);

int main(int argc, char **argv)
{
    extern char *optarg;
    extern int optind;
    int c, err = 0;
    int port = SERVICE_PORT;
    static char usage[] = "usage: %s [-d] [-p port]\n";

    while ((c = getopt(argc, argv, "dp:")) != -1)
        switch (c) {
        case 'p':
            port = atoi(optarg);
            /* if (port < 1024 || port > 65535) { */
            if (port < 0 || port > 65535) {
                fprintf(stderr, "invalid port number: %s\n", optarg);
                err = 1;
            }
            break;
        case '?':
            err = 1;
            break;
        }
    if (err || (optind < argc)) {
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }
    serve(port);
    return 0;
}

/* serve: set up the service */
void
serve(int port)
{
    int svc;        /* listening socket providing service */
    int rqst;       /* socket accepting the request */
    socklen_t alen;       /* length of address structure */
    struct sockaddr_in my_addr;    /* address of this service */
    struct sockaddr_in client_addr;  /* client's address */
    int sockoptval = 1;
    char hostname[128]; /* host name, for debugging */
    int n; /* message byte size */
    char buf[BUFSIZE];

    gethostname(hostname, 128);

    /* get a tcp/ip socket */
    /*   AF_INET is the Internet address (protocol) family  */
    /*   with SOCK_STREAM we ask for a sequenced, reliable, two-way */
    /*   conenction based on byte streams.  With IP, this means that */
    /*   TCP will be used */
    if ((svc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("cannot create socket");
        exit(1);
    }

    /* we use setsockopt to set SO_REUSEADDR. This allows us */
    /* to reuse the port immediately as soon as the service exits. */
    /* Some operating systems will not allow immediate reuse */
    /* on the chance that some packets may still be en route */
    /* to the port. */
    setsockopt(svc, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(int));

    /* set up our address */
    /* htons converts a short integer into the network representation */
    /* htonl converts a long integer into the network representation */
    /* INADDR_ANY is the special IP address 0.0.0.0 which binds the */
    /* transport endpoint to all IP addresses on the machine. */
    memset((char*)&my_addr, 0, sizeof(my_addr));  /* 0 out the structure */
    my_addr.sin_family = AF_INET;   /* address family */
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* bind to the address to which the service will be offered */
    if (bind(svc, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
        perror("bind failed");
        exit(1);
    }

    /* set up the socket for listening with a queue length of 5 */
    if (listen(svc, 5) < 0) {
        perror("listen failed");
        exit(1);
    }

    printf("server started on %s, listening on port %d\n", hostname, port);

    /* loop forever - wait for connection requests and perform the service */
    alen = sizeof(client_addr);     /* length of address */

    while (1) {
        rqst = accept(svc, (struct sockaddr *)&client_addr, &alen);

        if (rqst < 0) {
          perror("ERROR on accept");
          exit(1);
        }

        printf("received a connection from: %s port %d\n",
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        if (rqst < 0) {
          perror("ERROR on accept");
          exit(1);
        }

        /* Create child process */
        int pid = fork();

        if (pid < 0) {
          perror("ERROR on fork");
          exit(1);
        }

        if (pid == 0) {
          /* This is the client process */
          close(svc);
          while(doprocessing(rqst)){};
          exit(0);
        }
        else {
          close(rqst);
        }

        //shutdown(rqst, 2);    /* close the connection */
    }
}

/* read data from server and send data to client */
int
doprocessing (int rqst)
{
    int n; /* message byte size */
    char buf[BUFSIZE];
    /*
    One important thing to keep in mind is that TCP/IP
    sockets give you a byte stream, not a packet stream.
    If you write 20 bytes to a socket, the other side is not
    guaranteed to read 20 bytes in a read operation.
    It may read 20. It may read less if there was some packet fragmentation.
    In that case, you'll need to loop back and keep reading.
    If you write 20 bytes to a socket and then write another 20 bytes to a socket,
    the other side may read all 40 bytes at once ... or not.
    */
    /*
     * read: read input string from the client
     */
    bzero(buf, BUFSIZE);
    n = read(rqst, buf, BUFSIZE);
    if (n < 0)
      perror("ERROR reading from socket");
    printf("server received %d bytes: %s", n, buf);

    /*
     * write: echo the input string back to the client
     */
    n = write(rqst, buf, strlen(buf));
    if (n < 0)
      perror("ERROR writing to socket");

    return 1;
}
