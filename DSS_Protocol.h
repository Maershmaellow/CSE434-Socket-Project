#ifndef DSS_PROTOCOL_H
#define DSS_PROTOCOL_H

#include "util.h"
#include <vector>
#include <sstream>

using namespace std;

int parse_input(string input);
int register_disk(string disk_name, string IPv4_addr, int m_port, int c_port);
int register_user(string user_name, string IPv4_addr, int m_port, int c_port);
int config_dss(string dss_name, int n, int striping_unit);
int list();
int copy(string file_name, int file_size, string owner);
int read(string dss_name, string file_name, string user_name);
int disk_failure(string dss_name);
int deregister_user(string user_name);
int deregister_disk(string disk_name);
int decommission_dss(string dss_name);

#endif