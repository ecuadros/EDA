#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <iostream>
#include <algorithm> // sort algorithm
#include <functional> // for std::less, std::greater
#include <string>

template <typename T, typename V>
class NodeArray {
public:
    using value_type = T;
    using LinkedValueType = V;

    value_type m_key;
    LinkedValueType m_value;

    NodeArray(value_type key, LinkedValueType value)
        : m_key(key), m_value(value) {}

    value_type getData() const { return m_key; }
    value_type& getDataRef() { return m_key; }
    LinkedValueType getValue() const { return m_value; }
    LinkedValueType& getValueRef() { return m_value; }

    bool operator<(const NodeArray<T, V>& other) const {
        return m_key < other.m_key;
    }

    bool operator>(const NodeArray<T, V>& other) const {
        return m_key > other.m_key;
    }
};

template <typename T, typename V, typename CompareFn = std::less<NodeArray<T, V>>>
class CArray {
public:
    using value_type = T;
    using KeyType = T;
    using LinkedValueType = V;
    using Node = NodeArray<T, V>;
    using CompareFunc = CompareFn;
    using iterator = Node*;

private:
    Node* m_data;
    size_t m_size;
    CompareFunc m_compareFunc;

public:
    CArray(size_t size) : m_size(size) {
        m_data = new Node[size];
    }

    ~CArray() {
        delete[] m_data;
    }

    size_t size() const {
        return m_size;
    }

    iterator begin() {
        return m_data;
    }

    iterator end() {
        return m_data + m_size;
    }

    void sort() {
        std::sort(m_data, m_data + m_size, m_compareFunc);
    }
};

#endif  // __ARRAY_H__
