// Implements the server side of an echo client-server application program.
// The client reads ITERATIONS strings from stdin, passes the string to the
// this server, which simply sends the string back to the client.
//
// Compile on general.asu.edu as:
//   g++ -o server UDPEchoServer.c
//
// Only on general3 and general4 have the ports >= 1024 been opened for
// application programs.
#include <stdio.h>      // for printf() and fprintf()
#include <sys/socket.h> // for socket() and bind()
#include <arpa/inet.h>  // for sockaddr_in and inet_ntoa()
#include <stdlib.h>     // for atoi() and exit()
#include <string.h>     // for memset()
#include <unistd.h>     // for close()

#define ECHOMAX 255     // Longest string to echo

void DieWithError( const char *errorMessage ) // External error handling function
{
    perror( errorMessage );
    exit( 1 );
}

int main( int argc, char *argv[] )
{
    int sock;                        // Socket
    struct sockaddr_in echoServAddr; // Local address of server
    struct sockaddr_in echoClntAddr; // Client address
    unsigned int cliAddrLen;         // Length of incoming message
    char echoBuffer[ ECHOMAX ];      // Buffer for echo string
    unsigned short echoServPort;     // Server port
    int recvMsgSize;                 // Size of received message

    if( argc != 2 )         // Test for correct number of parameters
    {
        fprintf( stderr, "Usage:  %s <UDP SERVER PORT>\n", argv[ 0 ] );
        exit( 1 );
    }

    echoServPort = atoi(argv[1]);  // First arg: local port

    // Create socket for sending/receiving datagrams
    if( ( sock = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP ) ) < 0 )
        DieWithError( "server: socket() failed" );

    // Construct local address structure */
    memset( &echoServAddr, 0, sizeof( echoServAddr ) ); // Zero out structure
    echoServAddr.sin_family = AF_INET;                  // Internet address family
    echoServAddr.sin_addr.s_addr = htonl( INADDR_ANY ); // Any incoming interface
    echoServAddr.sin_port = htons( echoServPort );      // Local port

    // Bind to the local address
    if( bind( sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0 )
        DieWithError( "server: bind() failed" );

	printf( "server: Port server is listening to is: %d\n", echoServPort );

    for(;;) // Run forever
    {
        cliAddrLen = sizeof( echoClntAddr );

        // Block until receive message from a client
        if( ( recvMsgSize = recvfrom( sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &echoClntAddr, &cliAddrLen )) < 0 )
            DieWithError( "server: recvfrom() failed" );

        echoBuffer[ recvMsgSize ] = '\0';

        printf( "server: received string ``%s'' from client on IP address %s\n", echoBuffer, inet_ntoa( echoClntAddr.sin_addr ) );

        // Send received datagram back to the client
        if( sendto( sock, echoBuffer, strlen( echoBuffer ), 0, (struct sockaddr *) &echoClntAddr, sizeof( echoClntAddr ) ) != strlen( echoBuffer ) )
            DieWithError( "server: sendto() sent a different number of bytes than expected" );
    }
    // NOT REACHED */
}
