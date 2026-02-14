#include "CFour.h"
#include <iostream>

void printAll(CTwo* arr[], int n) {
    for (int i = 0; i < n; ++i) {
        arr[i]->print();
        std::cout << "-----------------" << std::endl;
    }
}

int main() {
    COne one(42, "hello");

    CTwo* arr[3];
    arr[0] = new CTwo(one, 3.14);
    arr[1] = new CThree(one, 2.71, "extra data");
    arr[2] = new CFour(one, 1.61, "deep extra", 7);

    printAll(arr, 3);

    for (int i = 0; i < 3; ++i) {
        delete arr[i];
    }

    return 0;
}