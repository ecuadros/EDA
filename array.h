#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <iostream>
#include <algorithm> // sort algorithm
#include <cassert>
#include "types.h"
#include "iterator.h"
using namespace std;

template <typename T, typename V>
class NodeArray
{
public:
  using value_type   = T;
  using Type      = T;
  using LinkedValueType = V;
private:
  using Node      = NodeArray<T, V> ;
public:
    value_type       m_key;
    LinkedValueType      m_value;

public:
    NodeArray(value_type key, LinkedValueType value) 
        : m_key(key), m_value(value) {}

    NodeArray(const NodeArray<T, V>& other) : 
        NodeArray(other.m_key, other.m_value) {}

    NodeArray(NodeArray<T, V>&& other) // Move constructor
        : m_key  (std::move(other.m_key)), 
          m_value(std::move(other.m_value)) {}
    NodeArray() {}

    NodeArray& operator=(const NodeArray& other) {
        if (this != &other) {
            m_key = other.m_key;
            m_value = other.m_value;
        }
        return *this;
    }

    value_type    getData() const   { return m_key; }
    value_type&   getDataRef()      { return m_key; }
    LinkedValueType  getValue() const { return m_value; }
    LinkedValueType& getValueRef()    { return m_value; }

    bool operator<(const NodeArray<T, V>& other) const { 
        return m_key < other.m_key;
    }
    // Error was here. Next line was missing
    bool operator>(const NodeArray<T, V>& other) const { 
        return m_key > other.m_key;
    }
};

template <typename _K, typename _V, 
            typename _CompareFn = std::less< NodeArray<_K, _V> & >>
struct ArrayTrait
{
    using  value_type      = _K;
    using  LinkedValueType = _V;
    using  Node      = NodeArray<_K, _V>;
    using  CompareFn = _CompareFn;
};

template <typename Container>
class array_forward_iterator
    : public general_iterator<Container, class array_forward_iterator<Container>> //
{
public:
    typedef class general_iterator<Container, array_forward_iterator<Container>> Parent;
    typedef typename Container::Node Node; //
    typedef array_forward_iterator<Container> myself;


public:
    array_forward_iterator(Container *pContainer, Node *pNode)
        : Parent(pContainer, pNode) {}
    array_forward_iterator(myself &other) : Parent(other) {}
    array_forward_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

public:
    array_forward_iterator operator++()
    {
        Parent::m_pNode++;
        return *this;
    }
};

template <typename Container>
class array_backward_iterator
    : public general_iterator<Container, class array_backward_iterator<Container>> //
{
public:
    typedef class general_iterator<Container, array_backward_iterator<Container>> Parent;
    typedef typename Container::Node Node; //
    typedef array_backward_iterator<Container> myself;
public:
    array_backward_iterator(Container *pContainer, Node *pNode)
        : Parent(pContainer, pNode) {}
    array_backward_iterator(myself &other) : Parent(other) {}
    array_backward_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

public:
    array_backward_iterator operator++()
    {
        Parent::m_pNode--;
        return *this;
    }
};

using TraitArrayFloatString = ArrayTrait<float, string>;
using TraitArrayIntInt = ArrayTrait<TX, int, std::greater<NodeArray<TX, int> &>>;
using TraitFloatLong = ArrayTrait<float, long, std::greater<NodeArray<float, long> &>>;

// Created by: @ecuadros
template <typename Traits>
class CArray
{
public:
    using value_type = typename Traits::value_type;
    using KeyType = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using Node = typename Traits::Node;
    using CompareFn = typename Traits::CompareFn;
    using myself = CArray<Traits>;
    using iterator = array_forward_iterator<myself>;
    using riterator = array_backward_iterator<myself>; // riterator means reverse_iterator
private:
    Node *m_pVect = nullptr;
    size_t m_vcount = 0, m_vmax = 0;
    string m_name = "Empty";

public:
    CArray() : m_name("Empty") {}
    CArray(string name) : m_name(name) {}

    ~CArray()
    {
        cout << "Destroying " << m_name << "..." << endl;
        destroy();
    }

    void insert(const value_type &key, LinkedValueType value)
    {
        if (m_vcount == m_vmax) // Array is already full?
            resize();
        m_pVect[m_vcount++] = Node(key, value);
        // cout << "Key=" << key << " Value=" << value << "\tinserted, m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
    }

    // DONE: Return last element (Like c++ std::vector)
    Node& back()
    {
        return m_pVect[m_vcount - 1];
    }

    // DONE: Remove the last element only
    // No reducimos la capacidad, solo el tamaño
    void pop_back()
    {
        assert(m_vcount > 0);
        value_type empty;
        m_vcount--;
        m_pVect[m_vcount] = Node(empty, empty);
    }

    // DONE: Pop from a specific position
    void pop(size_t i)
    {
        assert(m_vcount > 0);
        assert(i >= 0);
        assert(i < m_vcount);
        if(i == m_vcount - 1) {
            pop_back();
            return;
        }
        
        for(size_t j = i; j < m_vcount - 1; j++) {
            m_pVect[j] = m_pVect[j + 1];
        }
        m_vcount--;
    }

    // DONE: Pop from front
    void pop_front()
    {
        pop(0);
    }

    Node& getNode(size_t i) {
        return m_pVect[i];
    }

    void resize();

    void destroy()
    {
        delete[] m_pVect;
        m_pVect = nullptr;
        m_vcount = 0;
        m_vmax = 0;
    }

    void print(ostream &os)
    {
        // os << "Printing: " << m_name << endl;
        os << "Size: " << m_vcount << " Capacity: " << m_vmax << endl;
        // sort(m_pVect, m_pVect+m_vcount, CompareFn() );
        for (size_t i = 0; i < m_vcount; ++i)
            os << m_pVect[i].getData() << "\t: " << m_pVect[i].getValue() << endl;
        // os << "m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
    }

    void read(istream &is)
    {
        // freeing up space if it was already assigned
        destroy();
        // read here By Edson Caceres
        size_t vcount;
        is >> vcount >> m_vmax;
        Node *pTemp = new Node[m_vmax];
        // inserting values from .txt file
        value_type value;
        while (is >> value && size() != vcount)
        { // keeping in mind the m_vmax
            this->insert(value);
        }
    }

    size_t size()
    {
        return m_vcount;
    }

    value_type &operator[](size_t pos)
    {
        return m_pVect[pos].getDataRef();
    }

    iterator begin()
    {
        iterator iter(this, m_pVect);
        return iter;
    }

    iterator end()
    {
        iterator iter(this, m_pVect + m_vcount);
        return iter;
    }

    riterator rbegin()
    {
        riterator iter(this, m_pVect + m_vcount - 1);
        return iter;
    }
    
    riterator rend()
    {
        riterator iter(this, m_pVect - 1);
        return iter;
    }

    template <typename T>
    friend ostream &operator<<(ostream &os, CArray<T> &obj);

    template <typename T>
    friend istream &operator>>(istream &is, CArray<T> &obj);
};

template <typename Traits>
void CArray<Traits>::resize()
{
    Node *pTemp = new Node[m_vmax + 10];
    for (size_t i = 0; i < m_vcount; ++i)
        pTemp[i] = m_pVect[i];
    // *(pTemp+i) = m_pVect[i];
    // pTemp[i]   = *(m_pVect+i);
    // *(pTemp+i) = *(m_pVect+i);
    // *(i+pTemp) = m_pVect[i];
    // i[pTemp]   = m_pVect[i];
    delete[] m_pVect;
    m_pVect = pTemp;
    m_vmax += 10;
    // cout << "Vector resized m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
}

template <typename T>
ostream &operator<<(ostream &os, CArray<T> &obj)
{
    obj.print(os);
    return os;
}

template <typename T>
istream &operator>>(istream &is, CArray<T> &obj)
{
    // DONE: Read from stream
    using value_type = typename CArray<T>::value_type;
    using key_type = typename CArray<T>::KeyType;
    key_type key;
    value_type val;
    is >> key >> val;
    obj.insert(key, val);
    return is;
}

#endif // __ARRAY_H__
