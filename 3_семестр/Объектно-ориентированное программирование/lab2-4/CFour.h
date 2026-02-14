#ifndef CFOUR_H
#define CFOUR_H

#include "CThree.h"

class CFour : public CThree {
private:
    int flag;
public:
    CFour();
    CFour(const COne& obj, double d_, const std::string& e, int f);
    CFour(const CFour& other);
    ~CFour();

    CFour& operator=(const CFour& other);

    void setFlag(int f);
    int getFlag() const;

    void print() const override;
};

#endif
