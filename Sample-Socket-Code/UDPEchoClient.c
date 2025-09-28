// Implements the client side of an echo client-server application program.
// The client reads ITERATIONS strings from stdin, passes the string to the
// server, which simply echoes it back to the client.
//
// Compile on general.asu.edu as:
//   g++ -o client UDPEchoClient.c
//
// Only on general3 and general4 have the ports >= 1024 been opened for
// application programs.
#include <stdio.h>      // for printf() and fprintf()
#include <sys/socket.h> // for socket(), connect(), sendto(), and recvfrom()
#include <arpa/inet.h>  // for sockaddr_in and inet_addr()
#include <stdlib.h>     // for atoi() and exit()
#include <string.h>     // for memset()
#include <unistd.h>     // for close()

#define ECHOMAX 255     // Longest string to echo
#define ITERATIONS	5   // Number of iterations the client executes

void DieWithError( const char *errorMessage ) // External error handling function
{
    perror( errorMessage );
    exit(1);
}

int main( int argc, char *argv[] )
{
    size_t nread;
    int sock;                        // Socket descriptor
    struct sockaddr_in echoServAddr; // Echo server address
    struct sockaddr_in fromAddr;     // Source address of echo
    unsigned short echoServPort;     // Echo server port
    unsigned int fromSize;           // In-out of address size for recvfrom()
    char *servIP;                    // IP address of server
    char *echoString = NULL;         // String to send to echo server
    size_t echoStringLen = ECHOMAX;               // Length of string to echo
    int respStringLen;               // Length of received response

    echoString = (char *) malloc( ECHOMAX );

    if (argc < 3)    // Test for correct number of arguments
    {
        fprintf( stderr, "Usage: %s <Server IP address> <Echo Port>\n", argv[0] );
        exit( 1 );
    }

    servIP = argv[ 1 ];  // First arg: server IP address (dotted decimal)
    echoServPort = atoi( argv[2] );  // Second arg: Use given port

    printf( "client: Arguments passed: server IP %s, port %d\n", servIP, echoServPort );

    // Create a datagram/UDP socket
    if( ( sock = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP ) ) < 0 )
        DieWithError( "client: socket() failed" );

    // Construct the server address structure
    memset( &echoServAddr, 0, sizeof( echoServAddr ) ); // Zero out structure
    echoServAddr.sin_family = AF_INET;                  // Use internet addr family
    echoServAddr.sin_addr.s_addr = inet_addr( servIP ); // Set server's IP address
    echoServAddr.sin_port = htons( echoServPort );      // Set server's port

	// Pass string back and forth between server ITERATIONS times

	printf( "client: Echoing strings for %d iterations\n", ITERATIONS );

    for( int i = 0; i < ITERATIONS; i++ )
    {
        printf( "\nEnter string to echo: \n" );
        if( ( nread = getline( &echoString, &echoStringLen, stdin ) ) != -1 )
        {
            echoString[ (int) strlen( echoString) - 1 ] = '\0'; // Overwrite newline
            printf( "\nclient: reads string ``%s''\n", echoString );
        }
        else
            DieWithError( "client: error reading string to echo\n" );

        // Send the string to the server
        if( sendto( sock, echoString, strlen( echoString ), 0, (struct sockaddr *) &echoServAddr, sizeof( echoServAddr ) ) != strlen(echoString) )
       		DieWithError( "client: sendto() sent a different number of bytes than expected" );

        // Receive a response
        fromSize = sizeof( fromAddr );

        if( ( respStringLen = recvfrom( sock, echoString, ECHOMAX, 0, (struct sockaddr *) &fromAddr, &fromSize ) ) > ECHOMAX )
            DieWithError( "client: recvfrom() failed" );

        echoString[ respStringLen ] = '\0';

        if( echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr )
            DieWithError( "client: Error: received a packet from unknown source.\n" );

 		printf( "client: received string ``%s'' from server on IP address %s\n", echoString, inet_ntoa( fromAddr.sin_addr ) );
    }
    
    close( sock );
    exit( 0 );
}
