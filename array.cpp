#include <iostream>
#include "array.h"
using namespace std;

CArray::CArray(string name) 
       : m_name(name){
}

CArray::CArray() : CArray("Empty") {
}

CArray::~CArray(){
    cout << "Destroying " << m_name << "..." << endl;
    delete [] m_pVect;
    m_pVect = nullptr;
    m_vcount = 0;
    m_vmax = 0;
}

void CArray::resize(){
    value_type *pTemp = new value_type[m_vmax+10];
    for(size_t i = 0 ; i < m_vcount ; ++i)
        pTemp[i]   = m_pVect[i];
        // *(pTemp+i) = m_pVect[i];
        // pTemp[i]   = *(m_pVect+i);
        // *(pTemp+i) = *(m_pVect+i);
        // *(i+pTemp) = m_pVect[i];
        // i[pTemp]   = m_pVect[i];
    delete [] m_pVect;
    m_pVect = pTemp;
    m_vmax +=10;
    // cout << "Vector resized m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
}

void CArray::insert(value_type val){
    if(m_vcount == m_vmax) // Array is already full?
        resize();
    m_pVect[m_vcount++] = val;
    cout << "Val=" << val << " inserted, m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
}

void CArray::print(ostream &os){
    // os << "Printing: " << m_name << endl;
    os << m_vcount << " " << m_vmax << endl;
    for(size_t i = 0; i < m_vcount ; ++i )
        os << m_pVect[i] << endl;
    //os << "m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
}

ostream &operator<<(ostream &os, CArray &obj){
    obj.print(os);
    return os;
}

