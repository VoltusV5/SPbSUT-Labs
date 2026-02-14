#ifndef CONE_H
#define CONE_H

#include <iostream>
#include <string>

class COne {
private:
    int n;
    std::string s;
public:
    COne();
    COne(int n_, const std::string& s_);
    COne(const COne& other);
    ~COne();

    COne& operator=(const COne& other);

    int getN() const;
    void setN(int n_);

    std::string getS() const;
    void setS(const std::string& s_);

    void print() const;
};

#endif
