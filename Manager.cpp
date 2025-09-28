#include "util.h";
#include <vector>

using namespace std;

class Manager {
    private:
        vector<Disk> dss;

    public:
        int register_disk(string disk_name, string IPv4_addr, int m_port, int c_port) {

        }

        int register_user(string disk_name, string IPv4_addr, int m_port, int c_port) {
            
        }

        int config_dss(string dss_name, int n, int striping_unit) {

        }

        int list() {

        }

        int copy(string file_name, int file_size, string owner) {

        }

        int read(string dss_name, string file_name, string user_name) {

        }

        int disk_failure(string dss_name) {

        }

        int deregister_user(string user_name) {

        }

        int deregister_disk(string disk_name) {

        }

        int decommission_dss(string dss_name) {

        }
                
};

int main(int argc, char* argv[]) {

    int port_num = atoi(argv[1]);

    if(argc != 2) {
        cout << "Incorrect usage of command line arguments\n";
        return -1;
    } else if(port_num < 1730 || port_num > 2229) {
        cout << "port number must be in range [1730, 2229]\n";
        return -1;
    }

    while(1) {
        cout << "worked\n";
        break;
    }
}