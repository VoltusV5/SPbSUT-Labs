#include "CTwo.h"

CTwo::CTwo() : p(nullptr), d(0.0) {}

CTwo::CTwo(const COne& obj, double d_) : d(d_) {
    p = new COne(obj);
}

CTwo::CTwo(const CTwo& other) : d(other.d) {
    if (other.p)
        p = new COne(*other.p);
    else
        p = nullptr;
}

CTwo::~CTwo() {
    std::cout << "~CTwo" << std::endl;
    delete p;
}

CTwo& CTwo::operator=(const CTwo& other) {
    if (this != &other) {
        d = other.d;
        delete p;
        p = (other.p) ? new COne(*other.p) : nullptr;
    }
    return *this;
}

double CTwo::getD() const { return d; }
void CTwo::setD(double d_) { d = d_; }

COne* CTwo::getP() const { return p; }
void CTwo::setP(const COne& obj) {
    delete p;
    p = new COne(obj);
}

void CTwo::print() const {
    std::cout << "CTwo: d=" << d << std::endl;
    if (p) p->print();
}
