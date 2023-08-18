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

using XTraitFloatStringDesc = XTrait<float, string, std::less<KeyNode<int, int > &>>;
using XTraitIntIntAsc       = XTrait<int  , int   , std::greater<KeyNode<int, int > &>>;

// Created by: @ecuadros
template <typename Traits>
class CHeap{
public:
    using value_type      = typename Traits::value_type;
    using KeyType         = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using Node            = typename Traits::Node;
    using CompareFn       = typename Traits::CompareFn;
    using myself          = CHeap<Traits>;
private:
    CArray<Traits>    m_heap;
    string    m_name = "Empty";
    CompareFn compareFn;

    void heapifyAsc(size_t i){
        size_t parent = (i - 1) / 2;
        // TODO: Use CompareFn
        if (compareFn(m_heap[parent], m_heap[i])){
            swap(m_heap[parent], m_heap[i]);
            heapifyAsc(parent);
        }
    }

    void heapifyDesc(size_t i){
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        size_t smallest = i;
        // // Use CompareFn
        if (left < m_heap.size() && compareFn(m_heap[i], m_heap[left])){
            smallest = left;
        }
        if (right < m_heap.size() && compareFn(m_heap[smallest], m_heap[right])){
            smallest = right;
        }
        if (smallest != i){
            swap(m_heap[i], m_heap[smallest]);
            heapifyDesc(smallest);
        }
    }

public:
    CHeap(string name)  : m_name(name){ destroy();  }
    CHeap()                           { destroy();  }
    virtual ~CHeap(){
        cerr << "Destroying " << m_name << "..." << endl;
        destroy();
    }
    void destroy(){
        m_heap.destroy();
        m_heap.insert(0, 0);
    }

    // CHeap operator=(CHeap &other){
    //     destroy();
        
    // }
    
    void insert(const value_type &key, LinkedValueType value){
        m_heap.insert(key, value);
        heapifyAsc();
        // cout << "Key=" << key << " Value=" << value << "\tinserted, m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
    }

    // TODO: complete heapifyAsc function (useful for insertion)
    void heapifyAsc(){
        heapifyAsc(m_heap.size() - 1);
    }

    // TODO: complete heapifyDesc function (useful when we remove an element)
    void heapifyDesc(){
        heapifyDesc(0);
    }

    Node pop(){
        assert(m_heap.size() > 1);
        Node ans = m_heap[1];
        swap(m_heap[1], m_heap[m_heap.size() - 1]);
        m_heap.pop_back();
        heapifyDesc();
        return ans;
    }

    void print        (ostream &os){
        os << m_heap;
    }
    void read(istream &is){
        is >> m_heap;
    }

    size_t size()
    {  return m_heap.size();    }

    // TODO : agregar el operator value_type &
    value_type &operator[](size_t pos)
    {   return m_heap[pos].getDataRef();    }

};

#endif
