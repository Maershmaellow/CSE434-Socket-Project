#include <string>
#include <iostream>
#include <sys/socket.h> // for socket(), connect(), sendto(), and recvfrom()
#include <arpa/inet.h>  // for sockaddr_in and inet_addr()
#include <cstring>

#define MSGMAX 255

enum Return_Code {
    FAILURE = -1,
    SUCCESS = 0
};

enum State {
    Free = 0
};

// Cannot make a vector<type_struct> without a temporary value to put into the vector, for this reason Disk is a class not a struct
class Client {
    public:
        std::string name;
        std::string IP;
        unsigned short m_port;
        unsigned short c_port;
        State state;
};

