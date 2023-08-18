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

using XTraitFloatStringDesc = XTraitTrait<float, string, std::less<KeyNode<int, int > &>>;>;
using XTraitIntIntAsc       = XTraitTrait<int  , int   , std::greater<KeyNode<int, int > &>>;

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
public:
    CHeap(string name)  : m_name(name){ destroy();  }
    CHeap()                           { destroy();  }
    virtual ~CHeap(){
        cerr << "Destroying " << m_name << "..." << endl;
        reset();
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
        // Use CompareFn
    }

    // TODO: complete heapifyDesc function (useful when we remove an element)
    void heapifyDesc(){
        // Use CompareFn
    }

    Node pop(){
        assert(m_heap.size() > 1);
        Node ans = m_heap[1];
        swap(m_heap[1], m_heap[m_heap.size()-1]);
        m_heap.pop_back();
        heapifyDesc();
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
