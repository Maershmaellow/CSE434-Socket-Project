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
    int listener_port;
    cout << "Enter port for Manager to listen to: " << '\n';
    cin >> listener_port;

    while(1) {
        //listen to port
    }
}