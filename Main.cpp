#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    char* input;
    cout << "type a string" << '\n';
    cin >> input;
    cout << "String: " << *input << " is " << sizeof(*input)/sizeof(char) << " characters long";


    cout << "\nv1.0\n"; 
}