#include "util.h"


using namespace std;
///////////////////////////////////////////////////////////////////////////////////////
//                  UTILITY FUNCTIONS AND CLASSES
//////////////////////////////////////////////////////////////////////////////////////

class DSS_Protocol {
    private:
        vector<DSS*> dss_v;
        vector<Client*> user_v;
        vector<Client*> disk_v;
    public:
        DSS_Protocol(){}
        int parse_input(string input) {
            stringstream in_stream(input);
            vector<string> delimited;
            string temp;

            cout << "Delimiting\n";
            while(in_stream >> temp) {
                delimited.push_back(temp);
            }
            cout << "Delimited without errors\n";

            // switch case doesnt work in c++ for strings or char* ;(
            // delimit user input by spaces, then redirect to the appropriate function according to the typed command  
            if(delimited[0] == "register-disk" && delimited.size() == 5) {
                return register_disk(delimited[1], delimited[2], stoi(delimited[3]), stoi(delimited[4]));
            } else if(delimited[0] == "register-user" && delimited.size() == 5) {
                return register_user(delimited[1], delimited[2], stoi(delimited[3]), stoi(delimited[4]));
            } else if(delimited[0] == "configure-dss" && delimited.size() == 4) {
                return config_dss(delimited[1], stoi(delimited[2]), delimited[3]);
            } else if(delimited[0] == "ls" && delimited.size() == 1) {
                return list();
            } else if(delimited[0] == "copy" && delimited.size() == 4) {
                return copy(delimited[1], stoi(delimited[2]), delimited[3]);
            } else if(delimited[0] == "read" && delimited.size() == 4) {
                return read(delimited[1], delimited[2], delimited[3]);
            } else if(delimited[0] == "disk-failure" && delimited.size() == 2) {
                return disk_failure(delimited[1]);
            } else if(delimited[0] == "deregister-disk" && delimited.size() == 2) {
                return deregister_disk(delimited[1]);
            } else if(delimited[0] == "deregister-user" && delimited.size() == 2) {
                return deregister_user(delimited[1]);
            } else if(delimited[0] == "deregister-disk" && delimited.size() == 2) {
                return decommission_dss(delimited[1]);
            } else {
                cout << "Please type a valid command\n";
                return FAILURE;
            }

            return FAILURE;
        }

        // Protocol functions according to the expected behavior listed in the project description
        int register_disk(string disk_name, string IPv4_addr, int m_port, int c_port) {
            cout << "Registering Disk: " << disk_name << "\nIPv4 Address: " << IPv4_addr << "\nManagement Port: " << m_port << "\nCommunication Port: " << c_port << "\n";

            if(disk_name.length() > 15) {
                cout << "Disk name is too long, try again\n";
                return FAILURE;
            }
            for(Client* curr : disk_v) {
                if(disk_name == curr->name || m_port == curr->m_port || c_port == curr->c_port) {
                    cout << "Please use a unique name, management port, and communication port\n";
                    return FAILURE;
                }
            }

            Client* newClient = new Client(disk_name, IPv4_addr, m_port, c_port, Idle);
            cout << "successfully added new client\n";

            disk_v.push_back(newClient);
            return SUCCESS;
        }

        int register_user(string user_name, string IPv4_addr, int m_port, int c_port) {
            cout << "Registering User: " << user_name << "\nIPv4 Address: " << IPv4_addr << "\nManagement Port: " << m_port << "\nCommunication Port: " << c_port << "\n";

            if(user_name.length() > 15) {
                cout << "Disk name is too long, try again\n";
                return FAILURE;
            }
            for(Client* curr : user_v) {
                if(user_name == curr->name || m_port == curr->m_port || c_port == curr->c_port) {
                    cout << "Please use a unique name, management port, and communication port\n";
                    return FAILURE;
                }
            }

            Client* newClient = new Client(user_name, IPv4_addr, m_port, c_port, Idle);
            cout << "successfully added new client\n";

            user_v.push_back(newClient);
            return SUCCESS;
        }

        int config_dss(string dss_name, int n, string striping_unit) {
            int stripe_size;
            cout << "Configuring: " << dss_name << "\nNum Disks: " << n << "\nStriping Unit: " << striping_unit << "\n";
            if(striping_unit.find("M") > 0 ) {
                stripe_size = stoi(striping_unit) * 1000000;         // convert from MB to millions of bytes
            }else if(striping_unit.find("K") > 0) {
                stripe_size = stoi(striping_unit) * 1000;            // convert from KB to thousands of bytes
            }else {
                stripe_size = stoi(striping_unit);
            }


            if(dss_name.length() > 15) {
                cout << "Disk name is too long, try again\n";
                return FAILURE;
            } else if(n < 3) {
                cout << "A DSS must have n >= 3 disks\n";
                return FAILURE;
            } else if ((stripe_size & (stripe_size-1)) == 0 && (stripe_size > 1000000 || stripe_size < 128)) {
                cout << "striping unit must be a power of 2 within 128 bytes and 1MB\n";
                return FAILURE;
            }

            int numFree = 0;
            for(Client* curr : disk_v) {
                if(numFree >= n) {
                    break;
                }else if(curr->state == Free) {
                    numFree++;
                }
            }
            for(DSS* curr : dss_v) {
                if(curr->name == dss_name) {
                    cout << "DSS must have a unique name\n";
                    return FAILURE;
                }
            }

            // DSS meets minimum requirements to be created
            vector<Client*> tempDisk_v;
            DSS* newDSS = new DSS(dss_name);

            int good_choice = 0;
            while(good_choice < n) {
                int rand_disk = rand() % (disk_v.size()+1);
                if(disk_v[rand_disk]->state == Free) {
                    disk_v[rand_disk]->state = InDSS;
                    tempDisk_v.push_back(disk_v[rand_disk]);
                    good_choice++;
                }
            }

            dss_v.push_back(newDSS);
            return SUCCESS;
        }

        int list() {
            cout << "listing files\n";
            return 1;
        }

        int copy(string file_name, int file_size, string owner) {
            cout << "Copying: " << file_name << "\nFrom owner: " << owner << "\nFilesize: " << file_size << "\n";
            return 1;
        }

        int read(string dss_name, string file_name, string user_name) {
            cout << "Reading: " << file_name << "\nFrom dss: " << dss_name << "\nTo user: " << user_name << "\n";
            return 1;
        }

        int disk_failure(string dss_name) {
            cout << "DISK FAILURE\n";
            return 1;
        }

        int deregister_user(string user_name) {
            cout << "Deregistering user: " << user_name << "\n";

            for(int i=0; i<user_v.size();i++) {
                if(user_v[i]->name == user_name) {
                    cout << "Deleting user\n";
                    user_v.erase(user_v.begin()+i);
                    delete(user_v[i]);
                    return SUCCESS;
                }
            }
            cout << "No such user [" << user_name << "] exists\n";
            return FAILURE;
        }

        int deregister_disk(string disk_name) {
            cout << "Deregistering disk: " << disk_name << "\n";
            for(int i=0; i<user_v.size();i++) {
                if(disk_v[i]->name == disk_name && disk_v[i]->state != InDSS) {
                    cout << "Deleting user\n";
                    disk_v.erase(disk_v.begin()+i);
                    delete(disk_v[i]);
                    return SUCCESS;
                }
            }
            cout << "No such FREE disk [" << disk_name << "] exists\n";
            return FAILURE;
        }

        int decommission_dss(string dss_name) {
            cout << "Decommissioning DSS\n";
            for(int i=0; i < dss_v.size(); i++) {
                if(dss_v[i]->name == dss_name) {
                    // Update states of disks in DSS to Free
                    for(Client* curr : dss_v[i]->disks) {
                        curr->state == Free;
                    }
                    // Delete DSS
                    dss_v.erase(dss_v.begin()+i);
                    delete(dss_v[i]);
                    return SUCCESS;
                }
            }
            return 1;
        }
};

void DieWithError( const char *errorMessage ) // External error handling function
{
    perror( errorMessage );
    exit( 1 );
}

////////////////////////////////////////////////////////////////////////////////////////
//                          CORE NETWORKING FUNCTIONALITY
////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
    int sock;                           // Socket
    struct sockaddr_in servAddr;    // Local address of server
    struct sockaddr_in clntAddr;    // Client address
    unsigned int cliAddrLen;            // Length of incoming message
    char recBuffer[ MSGMAX ];            // Buffer for echo string
    unsigned short servPort;            // Server port
    int recvMsgSize;                    // Size of received message

    servPort = atoi(argv[1]);
    
    DSS_Protocol* protocol = new DSS_Protocol();

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

        // Display command and process it
        printf( "Manager: received command ``%s'' from client on IP address %s\n", recBuffer, inet_ntoa( clntAddr.sin_addr ) );
        int success = protocol->parse_input(recBuffer);
        if(protocol->parse_input(recBuffer) == TERMINATE) {
            cout << "terminating Manager process\n";
            delete(protocol);
            return 0;
        }

        // Send received datagram back to the client
        if( sendto( sock, to_string(success).c_str(), strlen( to_string(success).c_str() ), 0, (struct sockaddr *) &clntAddr, sizeof( clntAddr ) ) != strlen( to_string(success).c_str() ) )
        {
            DieWithError( "server: sendto() sent a different number of bytes than expected" );
        }    
        cout << "\n\n";
    }
}