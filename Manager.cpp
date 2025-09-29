#include "util.h"

using namespace std;

int main(int argc, char* argv[]) {
    int sock;                           // Socket
    struct sockaddr_in servAddr;    // Local address of server
    struct sockaddr_in clntAddr;    // Client address
    unsigned int cliAddrLen;            // Length of incoming message
    char recBuffer[ MSGMAX ];            // Buffer for echo string
    unsigned short servPort;            // Server port
    int recvMsgSize;                    // Size of received message

    servPort = atoi(argv[1]);

    if(argc != 2) {
        cout << "Incorrect usage of command line arguments\n";
        return -1;
    } else if(servPort < 1730 || servPort > 2229) {
        cout << "port number must be in range [1730, 2229]\n";
        return -1;
    }

    if( ( sock = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP ) ) < 0 ) {
        DieWithError( "server: socket() failed" );
    }

    memset( &servAddr, 0, sizeof( servAddr ) ); // Zero out structure
    servAddr.sin_family = AF_INET;                  // Internet address family
    servAddr.sin_addr.s_addr = htonl( INADDR_ANY ); // Any incoming interface
    servAddr.sin_port = htons( servPort );      // Local port

    if( bind( sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0 ) {
        DieWithError( "server: bind() failed" );
    }

	printf( "server: Port server is listening to is: %d\n", servPort );

    for(;;) // Run forever
    {
        cliAddrLen = sizeof( clntAddr );

        // Block until receive message from a client
        if( ( recvMsgSize = recvfrom( sock, recBuffer, MSGMAX, 0, (struct sockaddr *) &clntAddr, &cliAddrLen )) < 0 )
            DieWithError( "server: recvfrom() failed" );

        recBuffer[ recvMsgSize ] = '\0';

        printf( "server: received string ``%s'' from client on IP address %s\n", recBuffer, inet_ntoa( clntAddr.sin_addr ) );

        // Send received datagram back to the client
        if( sendto( sock, recBuffer, strlen( recBuffer ), 0, (struct sockaddr *) &clntAddr, sizeof( clntAddr ) ) != strlen( recBuffer ) )
            DieWithError( "server: sendto() sent a different number of bytes than expected" );
    }
}