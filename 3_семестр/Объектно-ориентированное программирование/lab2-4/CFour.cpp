#include "CFour.h"

CFour::CFour() : CThree(), flag(0) {}

CFour::CFour(const COne& obj, double d_, const std::string& e, int f) 
    : CThree(obj, d_, e), flag(f) {}

CFour::CFour(const CFour& other) : CThree(other), flag(other.flag) {}

CFour::~CFour() {
    std::cout << "~CFour" << std::endl;
}

CFour& CFour::operator=(const CFour& other) {
    if (this != &other) {
        CThree::operator=(other);
        flag = other.flag;
    }
    return *this;
}

void CFour::setFlag(int f) { flag = f; }
int CFour::getFlag() const { return flag; }

void CFour::print() const {
    std::cout << "CFour: flag=" << flag << std::endl;
    CThree::print();
}
