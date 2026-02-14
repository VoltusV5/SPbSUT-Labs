#ifndef STRING_H
#define STRING_H

#include <iostream>
using namespace std;

class String {
private:
    char* data;
    int length;

public:
    String();
    String(const char* str);
    String(const String& other);
    ~String();

    int Length() const;
    void Copy(const String& str); 
    void Copy(const char* str);    
    String Substr(int index, int count);
    void Remove(int index, int count); 
    void Insert(char* s, int index); 
    void trim(); 
    void read();  
    void print() const;   
};

#endif
