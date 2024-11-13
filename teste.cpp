#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv) {
    ofstream file("./logs/log.txt");

    file << "tesajcia" << endl;
    file.close();
    return 0;
}
