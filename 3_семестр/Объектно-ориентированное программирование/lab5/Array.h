#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <stdexcept>

template <typename T>
class Stack {
private:
    T* data;     
    size_t size;   // размер стека
    size_t capacity; // Текущая ёмкость массива

    void resize() {
        size_t newCapacity = capacity * 2 + 1; 
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    Stack(size_t initCapacity = 10) : size(0), capacity(initCapacity) {
        if (initCapacity == 0) {
            data = nullptr;
        } else {
            data = new T[capacity];
        }
    }

    ~Stack() {
        delete[] data;
    }

    Stack(const Stack<T>& other) : size(other.size), capacity(other.capacity) {
        data = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    Stack<T>& operator=(const Stack<T>& other) {
        if (this == &other) return *this;
        delete[] data;
        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        return *this;
    }

    void push(const T& value) {
        if (size == capacity) {
            resize();
        }
        data[size++] = value;
    }

    void pop() {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        --size;
    }

    // Просмотр верхнего элемента 
    T& top() {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        return data[size - 1];
    }

    const T& top() const {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        return data[size - 1];
    }

    bool empty() const {
        return size == 0;
    }

    size_t getSize() const {
        return size;
    }

    // Метод для печати стека (для тестирования, от вершины к основанию)
    void print() const {
        std::cout << "[ ";
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << "] (top is right)" << std::endl;
    }
};

#endif