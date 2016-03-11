#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
    cout << "Waiting 10 seconds" << endl;
    sleep(10);
    for(int i = 0; i < 100; i++){
        int r = rand() % 2;
        if (r >= 1) {
            cout << r <<" is larger than 2" << endl;
        }
    }
    return 0;
};