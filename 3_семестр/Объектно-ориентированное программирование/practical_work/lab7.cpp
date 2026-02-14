#include <iostream>
#include <string>
using namespace std;

int main() {
    string str;
    int cap_old = str.capacity();
    int cap_cur;
    int count = 0;

    for (int i = 0; i < 1000; i++) {
        str.push_back('a'); 
        cap_cur = str.capacity();
        if (cap_cur != cap_old) {
            count++;
            cap_old = cap_cur;
        }
    }

    cout << "Number of extensions: " << count << endl;
    return 0;
}
