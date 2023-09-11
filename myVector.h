#include <iostream>
#include <stdexcept>

template <typename T>
class MyVector {
private:
    T* data;
    size_t capacity;
    size_t size;

public:    
    MyVector(size_t initialCapacity = 10) : data(new T[initialCapacity]), capacity(initialCapacity), size(0) {}
    ~MyVector() { delete[] data; }

    // Overloading [] operator to access an element by index
    T& operator[](size_t index) {
        if (index >= size) 
            throw std::out_of_range("Index out of range");        
        return data[index];
    }

    // Overloading {} operator for initialization
    void operator()(std::initializer_list<T> values) {
        if (values.size() > capacity) {
            delete[] data;
            capacity = values.size();
            data = new T[capacity];
        }
        size = 0;
        for (const T& value : values) 
            data[size++] = value;        
    }

    // Push an element to the back of the vector
    void push_back(const T& value) {
        if (size == capacity) {
            capacity *= 2;
            T* newData = new T[capacity];
            for (size_t i = 0; i < size; ++i) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
        }
        data[size++] = value;
    }

    // Get the current size of the vector
    size_t getSize() const {
        return size;
    }

    // Get the current capacity of the vector
    size_t getCapacity() const {
        return capacity;
    }
};
