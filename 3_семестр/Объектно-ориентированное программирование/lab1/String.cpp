#include "String.h"
#include <cstring>
#include <cctype>
#include <stdexcept>

String::String() : data(nullptr), length(0) {}

String::String(const char* str) {
    if (!str) {
        data = nullptr;
        length = 0;
    } else {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
    }
}

String::String(const String& other) {
    length = other.length;
    data = new char[length + 1];
    strcpy(data, other.data);
}

String::~String() {
    delete[] data;
}

int String::Length() const {
    return length;
}

void String::Copy(const String& str) {
    delete[] data;
    length = str.length;
    data = new char[length + 1];
    strcpy(data, str.data);
}

void String::Copy(const char* str) {
    delete[] data;
    length = strlen(str);
    data = new char[length + 1];
    strcpy(data, str);
}


String String::Substr(int index, int count) {
    if (index < 0 || index >= length)
        throw std::out_of_range("Substr: the index is out of range");
    if (count <= 0)
        throw std::invalid_argument("Substr: count must be > 0");

    if (index + count > length) count = length - index;

    char* sub = new char[count + 1];
    strncpy(sub, data + index, count);
    sub[count] = '\0';

    String result(sub);
    delete[] sub;
    return result;
}

void String::Remove(int index, int count) {
    if (index < 0 || index >= length)
        throw std::out_of_range("Remove: the index is out of range");
    if (count <= 0)
        throw std::invalid_argument("Remove: count must be > 0");

    if (index + count > length) count = length - index;

    char* newData = new char[length - count + 1];
    strncpy(newData, data, index);
    strcpy(newData + index, data + index + count);

    delete[] data;
    data = newData;
    length -= count;
}

void String::Insert(char* s, int index) {
    if (!s)
        throw std::invalid_argument("Insert: the row to insert is nullptr");
    if (index < 0 || index > length)
        throw std::out_of_range("Insert: The index is out of range");

    int insertLen = strlen(s);
    char* newData = new char[length + insertLen + 1];

    strncpy(newData, data, index);
    strcpy(newData + index, s);
    strcpy(newData + index + insertLen, data + index);

    delete[] data;
    data = newData;
    length += insertLen;
}


void String::trim() {
    if (!data) return;

    int start = 0;
    while (isspace((unsigned char)data[start])) start++;

    int end = length - 1;
    while (end >= start && isspace((unsigned char)data[end])) end--;

    int newLen = (end - start + 1 > 0) ? end - start + 1 : 0;

    char* newData = new char[newLen + 1];
    strncpy(newData, data + start, newLen);
    newData[newLen] = '\0';

    delete[] data;
    data = newData;
    length = newLen;
}

void String::read() {
    char buffer[1024];
    cin.getline(buffer, 1024);
    Copy(buffer);
}

void String::print() const {
    if (data) cout << data;
}
