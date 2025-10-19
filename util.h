#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <iostream>
#include <sys/socket.h> // for socket(), connect(), sendto(), and recvfrom()
#include <arpa/inet.h>  // for sockaddr_in and inet_addr()
#include <cstring>
#include <vector>
#include <sstream>

#define MSGMAX 255


enum Return_Code {
    FAILURE = -1,
    SUCCESS = 0,
    TERMINATE = 100
};

enum State {
    Free = 0,
    InDSS = 1,
    Idle = 2
};

// Cannot make a vector<type_struct> without a temporary value to put into the vector, for this reason Disk is a class not a struct
class Client {
    public:
        std::string name;
        std::string IP;
        unsigned short m_port;
        unsigned short c_port;
        State state;
        Client(std::string name, std::string IP, int m_port, int c_port, State state) {
            this->name = name;
            this->IP = IP;
            this->m_port = m_port;
            this->c_port = c_port;
            this->state = state;
        }
};

class DSS {
    public:
        std::vector<Client*> disks;
        std::string name;
        int n;
        unsigned int striping_unit;
        
        DSS(std::string name, int n, unsigned int striping_unit) {
            this->name = name;
            this->n = n;
            this->striping_unit = striping_unit;
        }
        void addDisks(std::vector<Client*> disks) {
            this->disks = disks;
        }
};

#endif