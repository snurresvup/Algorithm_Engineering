#include <iostream>

using namespace std;

int main() {
    for (int i = 0; i < 10 ; i++) {
        cout << std::to_string(i) << " " << std::to_string(i*2) << endl;
    }

    return 0;
}