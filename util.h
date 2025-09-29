#include <string>
#include <iostream>


enum Return_Code {
    FAILURE = -1,
    SUCCESS = 0
};

enum State {
    Free = 0
};

// Cannot make a vector<type_struct> without a temporary value to put into the vector, for this reason Disk is a class not a struct
class Disk {
    public:
        std::string disk_name;
        State state;
};

