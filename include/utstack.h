#pragma once

#include <iostream>

const double resizeCoeff = 2.0;

template<class T> class Vector {
private:
    T* resize(int n) {
        T* newData = new T[n];
        memSize = n;
        for (int i = 0; i < size; i++)
            newData[i] = data[i];
        delete[] data;
        data = newData;
        return data;
    }
protected:
    T* data;
    int size;
    int memSize;
public:
    ~Vector() {
        delete[] data;
    }
    Vector(int n = 0) {
        if (n < 0) throw "Invalid vector size";
        if (n == 0) {
            data = nullptr;
            size = 0;
            memSize = 0;
        }
        if (n > 0) {
            data = new T[n];
            size = n;
            memSize = n;
        }
    }
    Vector(const Vector& v) {
        int n = v.size;
        if (n == 0) {
            data = nullptr;
            size = 0;
            memSize = 0;
        }
        if (n > 0) {
            data = new T[n];
            size = n;
            memSize = n;
        }
        for (int i = 0; i < size; i++)
            data[i] = v.data[i];
    }
    void push_back(T elem) {
        if (size == memSize) {
            if (memSize == int(memSize * resizeCoeff)) resize(memSize + 1);
            else resize(memSize * resizeCoeff);
        }
        data[size] = elem;
        size++;
    }
    void pull_back() {
        size--;
    }
    void push_front(T elem) { //
        insert(elem, 0);
    }
    void pop_front() { //
        erase(0);
    }
    T& operator[](int index) {
        return data[index];
    }
    T operator[](int index) const {
        return data[index];
    }
    void insert(T elem, int index) {
        push_back(data[size - 1]);
        for (int i = size - 1; i > index; i--)
            data[i] = data[i - 1];
        data[index] = elem;
    }
    void erase(int index) {
        for (int i = index; i < size; i++)
            data[i] = data[i + 1];
        size--;
    }
    size_t getSize() const {
        return size;
    }
};

template<class T> class TStack: protected Vector<T> {
    int maxSize;
public:
	TStack();
	TStack(int maxs);
	TStack(const TStack& s);
	T top();
	void push(T);
	void pop();
	bool empty();
	bool full();
    size_t getMaxSize() const;
};

template<class T> TStack<T>::TStack() : Vector<T>() {
    maxSize = 0;
}

template<class T> TStack<T>::TStack(int maxs) : Vector<T>(0) {
    maxSize = maxs;
}

template<class T> TStack<T>::TStack(const TStack& s) : Vector<T>(s){
    maxSize = s.maxSize;
}

template<class T> T TStack<T>::top() {
    if (!empty())
        return data[size - 1];
    else throw "Stack empty";
}

template<class T> void TStack<T>::push(T elem) {
    if (!full())
        push_back(elem);
    else throw "Stack full";
}

template<class T> void TStack<T>::pop() {
    if (!empty())
        pull_back();
    else throw "Stack empty";
}

template<class T> bool TStack<T>::empty() {
    return (size == 0);
}

template<class T> bool TStack<T>::full() {
    return (size == maxSize);
}

template<class T> size_t TStack<T>::getMaxSize() const {
    return maxSize;
}