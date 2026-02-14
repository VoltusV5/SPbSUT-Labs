#include "CThree.h"

CThree::CThree() : CTwo(), extra("") {}

CThree::CThree(const COne& obj, double d_, const std::string& e) 
    : CTwo(obj, d_), extra(e) {}

CThree::CThree(const CThree& other) : CTwo(other), extra(other.extra) {}

CThree::~CThree() {
    std::cout << "~CThree" << std::endl;
}

CThree& CThree::operator=(const CThree& other) {
    if (this != &other) {
        CTwo::operator=(other);
        extra = other.extra;
    }
    return *this;
}

void CThree::setExtra(const std::string& e) { extra = e; }
std::string CThree::getExtra() const { return extra; }

void CThree::print() const {
    std::cout << "CThree: extra=" << extra << std::endl;
    CTwo::print();
}
