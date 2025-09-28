// Example of a structure that can be written into a socket.
// The structure could be defined to conform to the packet format required by
// the application program.

// Simply pass the address of the struct as the second parameter to sendto(),
// and the size of the structure, i.e., sizeof( struct sample ) as the third
// parameter to sendto().  Similarly for recvfrom().

struct sample
{
    char message[ 20 ];
    int n;
};
