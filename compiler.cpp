using namespace std;

#include <iostream>
#include <string>

int main(int argc, char **argv) {
    
    if (argc != 1) {
        cout << "You must input exactly one command line argument: the file you want to compile" << endl;
    }

    string filename = argv[0];

    return 0;
}