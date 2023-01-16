#include <iostream>
#include <cstring>
using namespace std;

int main(int argc, char **argv) {
    cout << "Number of command line arguments: " << (argc-1) << endl;
    for(int i = 1; i < argc; i++) {
        cout << "* Arg " << i << ": " << argv[i] << endl;
    }
    return 0;
}