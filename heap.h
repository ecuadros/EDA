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

using XTraitFloatStringDesc = XTrait<float, string, std::less<KeyNode<int, int> &>>;
using XTraitIntIntAsc = XTrait<int, int, std::greater<KeyNode<int, int> &>>;

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
    CompareFn compareFn;

    void heapifyAsc(size_t i)
    {
        size_t parentIndex = (i - 1) / 2;
        // TODO: Use CompareFn (GREATER)

        // Si el valor almacenado en el hijo es menor
        // entonces, el hijo cambia de posición con el
        // padre (subiendo).
        if (compareFn(m_heap[parentIndex], m_heap[i]))
        {
            swap(m_heap[parentIndex], m_heap[i]);
            heapifyAsc(parentIndex);
        }
    }

    void heapifyDesc(size_t i)
    {
        size_t parentIndex = i;
        size_t leftChildIndex = 2 * i + 1;
        size_t rightChildIndex = 2 * i + 2;
        size_t smallestChildrenIndex = 0;

        // compareFn llama al std::Greater, es decir si hijo izquierda
        // es mayor entonces el indice del hijo mas pequeño es derecha.
        if (compareFn (m_heap[leftChildIndex], m_heap[rightChildIndex]) )
            smallestChildrenIndex = rightChildIndex;
        else
            smallestChildrenIndex = leftChildIndex;
        // Si el hijo es menor que el padre intercambian posiciones y 
        // se realiza el heapifyDesc desde el menor valor.
        if (compareFn(m_heap[parentIndex], m_heap[smallestChildrenIndex]))
        {
            swap(m_heap[parentIndex], m_heap[smallestChildrenIndex]);
            heapifyDesc(smallestChildrenIndex);
        }
    }

public:
    CHeap(string name) : m_name(name) { destroy(); }
    CHeap() { destroy(); }
    virtual ~CHeap()
    {
        cerr << "Destroying " << m_name << "..." << endl;
        destroy();
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
        heapifyAsc();
    }

    // TODO: complete heapifyAsc function (useful for insertion)
    void heapifyAsc()
    {
        size_t lastElementIndex = m_heap.size() - 1;
        heapifyAsc(lastElementIndex);
    }

    // TODO: complete heapifyDesc function (useful when we remove an element)
    void heapifyDesc()
    {
        size_t firstElementIndex = 0;
        heapifyDesc(firstElementIndex);
    }

    Node pop()
    {
        assert(m_heap.size() > 1);
        Node ans = m_heap[1];
        swap(m_heap[1], m_heap[m_heap.size() - 1]);
        m_heap.pop_back();
        heapifyDesc();
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

    // TODO : agregar el operator value_type &
    value_type &operator[](size_t pos)
    {
        return m_heap[pos].getDataRef();
    }
};

#endif
