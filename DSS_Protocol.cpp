#include "DSS_Protocol.h"

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
    if(delimited[0] == "register-disk" && delimited.size() == 5) {
        register_disk(delimited[1], delimited[2], stoi(delimited[3]), stoi(delimited[4]));
    } else if(delimited[0] == "register-user" && delimited.size() == 5) {
        register_user(delimited[1], delimited[2], stoi(delimited[3]), stoi(delimited[4]));
    } else if(delimited[0] == "configure-dss" && delimited.size() == 4) {
        config_dss(delimited[1], stoi(delimited[2]), stoi(delimited[3]));
    } else if(delimited[0] == "ls" && delimited.size() == 1) {
        list();
    } else if(delimited[0] == "copy" && delimited.size() == 4) {
        copy(delimited[1], stoi(delimited[2]), delimited[3]);
    } else if(delimited[0] == "read" && delimited.size() == 4) {
        read(delimited[1], delimited[2], delimited[3]);
    } else if(delimited[0] == "disk-failure" && delimited.size() == 2) {
        disk_failure(delimited[1]);
    } else if(delimited[0] == "deregister-disk" && delimited.size() == 2) {
        deregister_disk(delimited[1]);
    } else if(delimited[0] == "deregister-user" && delimited.size() == 2) {
        deregister_user(delimited[1]);
    } else if(delimited[0] == "deregister-disk" && delimited.size() == 2) {
        decommission_dss(delimited[1]);
    } else {
        return FAILURE;
    }

    return 0;
}

int register_disk(string disk_name, string IPv4_addr, int m_port, int c_port) {
    cout << "Registering Disk: " << disk_name << "\nIPv4 Address: " << IPv4_addr << "\nManagement Port: " << m_port << "\nCommunication Port: " << c_port << "\n";
    return 1;
}

int register_user(string user_name, string IPv4_addr, int m_port, int c_port) {
    cout << "Registering User: " << user_name << "\nIPv4 Address: " << IPv4_addr << "\nManagement Port: " << m_port << "\nCommunication Port: " << c_port << "\n";
    return 1;
}

int config_dss(string dss_name, int n, int striping_unit) {
    cout << "Configuring: " << dss_name << "\nNum Disks: " << n << "\nStriping Unit: " << striping_unit << "\n";
    return 1;
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
    return 1;
}

int deregister_disk(string disk_name) {
    cout << "Deregistering disk: " << disk_name << "\n";
    return 1;
}

int decommission_dss(string dss_name) {
    cout << "Decommissioning DSS\n";
    return 1;
}