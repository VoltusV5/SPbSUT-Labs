#include <iostream>
#include <string>
using namespace std;

int main() {
    string str("012345");
    string sub = str.substr(1, 3);
    cout << "sub -> " << sub << endl;
    cout << "---------------------------------------" << endl;
    cout << "sub.length() = " << sub.length() << endl;
    cout << "---------------------------------------" << endl;
    cout << "sub.capacity() = " << sub.capacity() << endl;
    cout << "---------------------------------------" << endl;
    return 0;
}
