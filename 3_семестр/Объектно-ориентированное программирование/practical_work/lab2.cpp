#include <iostream>
#include <string>
using namespace std;

int main() {
    string str("1234");
    cout << "str -> " << str << endl;
    cout << "---------------------------------------" << endl;
    cout << "str.length() = " << str.length() << endl;
    cout << "---------------------------------------" << endl;
    cout << "str.capacity() = " << str.capacity() << endl;
    cout << "---------------------------------------" << endl;
    return 0;
}
