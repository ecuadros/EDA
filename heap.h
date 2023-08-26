#ifndef __HEAP_H__
#define __HEAP_H__

#include <iostream>
#include <algorithm> // swap algorithm
#include <cassert>
#include "types.h"
#include "keynode.h"
#include "xtrait.h"
#include "array.h"

using namespace std;

// Created by: @ecuadros
template <typename Traits>
class CHeap
{
public:
    using value_type = typename Traits::value_type;
    using KeyType = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using Node = typename Traits::Node;
    using CompareFn = typename Traits::CompareFn;
    using myself = CHeap<Traits>;

private:
    CArray<Traits> m_heap;
    string m_name = "Empty";

public:
    CHeap(string name) : m_name(name) { destroy(); }
    CHeap() { destroy(); }
    virtual ~CHeap()
    {
        cerr << "Destroying " << m_name << "..." << endl;
    }
    void destroy()
    {
        m_heap.destroy();
        m_heap.insert(0, 0);
    }

    // CHeap operator=(CHeap &other){
    //     destroy();

    // }

    void insert(const value_type &key, LinkedValueType value)
    {
        m_heap.insert(key, value);
        heapifyAsc(m_heap.size() - 1);

        // cout << "Key=" << key << " Value=" << value << "\tinserted, m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
    }

    // DONE: complete heapifyAsc function (useful for insertion)
    void heapifyAsc(size_t i)
    {
        size_t parentIdx = (i) / 2;
        if (parentIdx >= 1 && CompareFn()(m_heap.getNode(i), m_heap.getNode(parentIdx)))
        {
            swap(m_heap.getNode(i), m_heap.getNode(parentIdx));
            heapifyAsc(parentIdx);
        }
    }

    // DONE: complete heapifyDesc function (useful when we remove an element)
    void heapifyDesc(size_t i)
    {
        size_t left = 2 * i;
        size_t right = 2 * i + 1;
        size_t largest = i;
        value_type a = m_heap[left];
        // cout<<"Size: "<<m_heap.size() <<"Left:" << left << " Right: " <<right<<endl;
        if (left < m_heap.size() && CompareFn()(m_heap.getNode(left), m_heap.getNode(largest)))
        {
            largest = left;
        }
        if (right < m_heap.size() && CompareFn()(m_heap.getNode(right), m_heap.getNode(largest)))
        {
            largest = right;
        }
        if (largest != i)
        {
            swap(m_heap.getNode(i), m_heap.getNode(largest));
            heapifyDesc(largest);
        }
    }

    Node pop()
    {
        assert(m_heap.size() > 1);
        Node ans = m_heap.getNode(1);
        swap(m_heap.getNode(1), m_heap.getNode(m_heap.size() - 1));
        m_heap.pop_back();
        heapifyDesc(1);
        return ans;
    }

    void print(ostream &os)
    {
        os << m_heap;
    }

    void read(istream &is)
    {
        is >> m_heap;
    }

    size_t size()
    {
        return m_heap.size();
    }

    // DONE: agregar el operator [] que retorna value_type &
    value_type &operator[](size_t pos)
    {
        return m_heap[pos].getDataRef();
    }
};

#endif
