#include <iostream>
#include <string>
using namespace std;

int main() {
    string str;
    int cap_old = str.capacity();
    int cap_cur;

    for (int i = 0; i < 1000; i++) {
        str.push_back('a');
        cap_cur = str.capacity();
        if (cap_cur != cap_old) {
            cout << "length: " << str.length()
            << " | capacity has changed to: " << cap_cur << endl;
            cap_old = cap_cur;
        }
    }
    return 0;
}
