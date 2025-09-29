#include "util.h"
#include <unistd.h>

using namespace std;

void DieWithError( const char *errorMessage ) // External error handling function
{
    perror( errorMessage );
    exit( 1 );
}


int main(int argc, char* argv[]) {
    if(argc != 6) {
        cout << "incorrect number of command line parameters\n";
        return -1;
    }

    int sock;
    struct sockaddr_in managerAddr; // Echo server address
    struct sockaddr_in fromAddr;     // Source address of echo

    string diskName = argv[1];
    char* managerIP = argv[2];
    int manager_port = atoi(argv[3]);
    int m_port = atoi(argv[4]);
    int c_port = atoi(argv[5]);

    char *msgString = NULL;         // String to send to echo server
    size_t msgStringLen = MSGMAX;               // Length of string to echo
    int respStringLen;               // Length of received response

    if( ( sock = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP ) ) < 0 ) {
        DieWithError( "client: socket() failed" );
    }

    memset( &managerAddr, 0, sizeof( managerAddr ) ); // Zero out structure
    managerAddr.sin_family = AF_INET;                  // Use internet addr family
    managerAddr.sin_addr.s_addr = inet_addr( managerIP ); // Set server's IP address
    managerAddr.sin_port = htons( manager_port );      // Set server's port


    size_t nread;
    unsigned int fromSize;
    for(;;)
    {
        printf( "\nEnter string to echo: \n" );
        if( ( nread = getline( &msgString, &msgStringLen, stdin ) ) != -1 )
        {
            msgString[ (int) strlen( msgString) - 1 ] = '\0'; // Overwrite newline
            printf( "\nclient: reads string ``%s''\n", msgString );
        }
        else
            DieWithError( "client: error reading string to echo\n" );

        
        // Send the string to the server
        if( sendto( sock, msgString, strlen( msgString ), 0, (struct sockaddr *) &managerAddr, sizeof( managerAddr ) ) != strlen(msgString) )
       		DieWithError( "client: sendto() sent a different number of bytes than expected" );

        // Receive a response
        fromSize = sizeof( fromAddr );

        if( ( respStringLen = recvfrom( sock, msgString, MSGMAX, 0, (struct sockaddr *) &fromAddr, &fromSize ) ) > MSGMAX )
            DieWithError( "client: recvfrom() failed" );

        msgString[ respStringLen ] = '\0';

        if( managerAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr )
            DieWithError( "client: Error: received a packet from unknown source.\n" );

 		printf( "client: received string ``%s'' from server on IP address %s\n", msgString, inet_ntoa( fromAddr.sin_addr ) );
        cout << "\n";
    }
    
    close( sock );
    exit( 0 );
}