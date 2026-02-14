#include "String.h"
#include "Complex.h"
#include <iostream>
#include <stdexcept>
using namespace std;

int main() {
    try {
        cout << "=== String Class Test ===" << endl;
        String s1("   Hello World!   ");
        cout << "The original line: ";
        s1.print();
        cout << endl;

        s1.trim();
        cout << "After trim: ";
        s1.print();
        cout << endl;

        try {
            String sub = s1.Substr(0, 5);
            cout << "Substring (0, 5): ";
            sub.print();
            cout << endl;
        } catch (const out_of_range& ex) {
            cerr << "!!! Error when receiving a substring: " << ex.what() << endl;
        }

        try {
            s1.Insert((char*)" C++", 0);
            cout << "After Insert: ";
            s1.print();
            cout << endl;
        } catch (const invalid_argument& ex) {
            cerr << "!!! Insertion error: " << ex.what() << endl;
        }

        try {
            s1.Remove(10, 5);
            cout << "After Remove(0,6): ";
            s1.print();
            cout << endl;
        } catch (const out_of_range& ex) {
            cerr << "!!! Error when deleting: " << ex.what() << endl;
        }

        cout << "\n=== Complex Class Test ===" << endl;
        Complex c1(0, 0), c2(0, 0);

        try {
            Complex sum = c1 + c2;
            cout << "c1 + c2 = "; sum.print(); cout << endl;

            Complex diff = c1 - c2;
            cout << "c1 - c2 = "; diff.print(); cout << endl;

            Complex prod = c1 * c2;
            cout << "c1 * c2 = "; prod.print(); cout << endl;

            Complex quot = c1 / c2;
            cout << "c1 / c2 = "; quot.print(); cout << endl;

        } catch (const runtime_error& ex) {
            cerr << "!!! Error when working with complex numbers: " << ex.what() << endl;
        }
    
    } catch (const bad_alloc& ex) {
        cerr << "!!! Memory allocation error: " << ex.what() << endl;
    } catch (const exception& ex) {
        cerr << "!!! A common mistake: " << ex.what() << endl;
    }
    cout << "end of the programm" << endl;
    return 0;
}
