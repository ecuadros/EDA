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
using XTraitIntIntDes      = XTrait<TX,int, std::greater<KeyNode<TX, int > &>>;
using XTraitIntIntAsc       = XTrait<TX,int, std::less<KeyNode<TX, int > &>>;

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
    string    m_name = "Empty";
    CArray<Traits>    m_heap;
public:
    CHeap(string name)  : m_name(name){ destroy();  }
    CHeap()                           { destroy();  }
    virtual ~CHeap(){
        cerr << "Destroying " << m_name << "..." << endl;
        reset();
    }
    void reset(){
        m_heap.destroy();
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

    // This function sorted function (useful for insertion)
    void heapifyAsc(){
        auto flow=m_heap.size()-1;
        while (flow >1){
            Node& Child_Node =m_heap.GetNode(flow);
            flow=floor(flow/2);
            Node& Father_Node=m_heap.GetNode(flow);
            if(m_heap.CompareValue_(Child_Node,Father_Node)){
                Node tmp_Child_Node= Child_Node;
                Child_Node=Father_Node;
                Father_Node=tmp_Child_Node;
            }
        }    
    }
    void heapifyDesc(){
        auto flow=1;
        while (flow <= m_heap.size()-1){
            Node& Father_Node =m_heap.GetNode(flow);
            auto flow_i=2*flow+1;
            flow=2*flow;
            if(flow_i<=m_heap.size()-1){
                Node& Child_Node_1=m_heap.GetNode(flow);
                Node& Child_Node_2=m_heap.GetNode(flow_i);
                if(m_heap.CompareValue_(Child_Node_1,Child_Node_2)){
                    if(m_heap.CompareValue_(Child_Node_1,Father_Node)){
                         Node tmp_Child_Node= Child_Node_1;
                         Child_Node_1=Father_Node;
                         Father_Node=tmp_Child_Node;
                    }
                }
                else{
                    if(m_heap.CompareValue_(Child_Node_2,Father_Node)){
                        Node tmp_Child_Node= Child_Node_2;
                        Child_Node_2=Father_Node;
                        Father_Node=tmp_Child_Node;
                        flow=flow_i;
                    }
                 }
            }
            else{
                if(flow<=m_heap.size()-1){
                    Node& Child_Node_1=m_heap.GetNode(flow);
                    if(m_heap.CompareValue_(Child_Node_1,Father_Node)){
                        Node tmp_Child_Node= Child_Node_1;
                        Child_Node_1=Father_Node;
                        Father_Node=tmp_Child_Node;
                    }  
                }
                
                }
            }
        }    

    void pop_back(){
        assert(m_heap.size() > 1);
        Node& tmp1 =m_heap.GetNode(1);
        Node& tmp2 =m_heap.GetNode(m_heap.size()-1);
        swap(tmp1, tmp2);
        m_heap.pop_back();//Delete the last element;
        heapifyDesc();
    }
    Node back(){
        assert(m_heap.size() > 1);
        Node& tmp1 =m_heap.GetNode(1);
        Node& tmp2 =m_heap.GetNode(m_heap.size()-1);
        swap(tmp1, tmp2);
        Node tmp=m_heap.back();//Recover the last element;
        heapifyDesc();
        return tmp;
    }

    void print        (ostream &os){
        os << m_heap;
    }
    void read(istream &is){
        is >> m_heap;
    }

    size_t size()
    {  return m_heap.size();    }

    // : agregar el operator value_type &
    value_type &operator[](size_t pos)
    {   return m_heap[pos].getDataRef();}
};

#endif
