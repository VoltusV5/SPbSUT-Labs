#include "COne.h"

COne::COne() : n(0), s("") {}

COne::COne(int n_, const std::string& s_) : n(n_), s(s_) {}

COne::COne(const COne& other) : n(other.n), s(other.s) {}

COne::~COne() {
    std::cout << "~COne" << std::endl;
}

COne& COne::operator=(const COne& other) {
    if (this != &other) {
        n = other.n;
        s = other.s;
    }
    return *this;
}

int COne::getN() const { return n; }
void COne::setN(int n_) { n = n_; }

std::string COne::getS() const { return s; }
void COne::setS(const std::string& s_) { s = s_; }

void COne::print() const {
    std::cout << "COne: n=" << n << ", s=" << s << std::endl;
}
