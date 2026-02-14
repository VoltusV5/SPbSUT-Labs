#ifndef CTHREE_H
#define CTHREE_H

#include "CTwo.h"

class CThree : public CTwo {
private:
    std::string extra;
public:
    CThree();
    CThree(const COne& obj, double d_, const std::string& e);
    CThree(const CThree& other);
    ~CThree();

    CThree& operator=(const CThree& other);

    void setExtra(const std::string& e);
    std::string getExtra() const;

    void print() const override;
};

#endif
