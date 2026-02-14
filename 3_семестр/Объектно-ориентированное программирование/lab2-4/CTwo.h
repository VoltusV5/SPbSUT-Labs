#ifndef CTWO_H
#define CTWO_H

#include "COne.h"

class CTwo {
protected:
    COne* p;
    double d;
public:
    CTwo();
    CTwo(const COne& obj, double d_);
    CTwo(const CTwo& other);
    virtual ~CTwo();

    CTwo& operator=(const CTwo& other);

    double getD() const;
    void setD(double d_);

    COne* getP() const;
    void setP(const COne& obj);

    virtual void print() const;
};

#endif
