#include "Complex.h"
#include "Array.h"
#include <iostream>
using namespace std;

int main() {
    cout << "=== Test Complex<T> ===" << endl;
    Complex<int> ci1(3, 4), ci2(1, -2);
    Complex<int> ci_sum = ci1 + ci2;
    cout << "ci1 + ci2 = "; ci_sum.print(); cout << endl;

    Complex<double> cd1(2.5, 1.5), cd2(1.0, -0.5);
    Complex<double> cd_mul = cd1 * cd2;
    cout << "cd1 * cd2 = "; cd_mul.print(); cout << endl;

    cout << "\n=== Test Stack<T> ===" << endl;
    Stack<int> stk(5); 

    for (int i = 0; i < 7; ++i) {  
        stk.push(i * 10);
    }
    cout << "Stack after pushes: ";
    stk.print();  

    cout << "Top: " << stk.top() << endl; 
    stk.pop();
    cout << "After pop, top: " << stk.top() << endl; 

    try {
        while (!stk.empty()) stk.pop();
        stk.pop();  
    } catch (const exception& e) {
        cout << "Exception: " << e.what() << endl; 
    }

    return 0;
}
