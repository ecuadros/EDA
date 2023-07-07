#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <iostream>
using namespace std;

template <typename T>
class CArray{
private:
    // typedef typename Traits::T          value_type;
    typedef T value_type;
    value_type *m_pVect = nullptr;
    size_t  m_vcount = 0, m_vmax = 0;
    string  m_name = "Empty";
public:
    CArray(): m_name("Empty"){}
    CArray(string name) : m_name(name) {}
    ~CArray(){ 
        cout << "Destroying " << m_name << "..." << endl;
        delete [] m_pVect;
        m_pVect = nullptr;
        m_vcount = 0;
        m_vmax = 0;
    }
    void insert(value_type val){
        if(m_vcount == m_vmax) // Array is already full?
            resize();
        m_pVect[m_vcount++] = val;
        cout << "Val=" << val << " inserted, m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
    }
    void resize       ();
    void print        (ostream &os){
        // os << "Printing: " << m_name << endl;
        os << m_vcount << " " << m_vmax << endl;
        for(size_t i = 0; i < m_vcount ; ++i )
            os << m_pVect[i] << endl;
        //os << "m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
    }
    void cleanArray () {
        cout << "Cleaning array..." << endl;
        delete [] m_pVect;
        m_pVect = nullptr;
        cout << "Array cleaned!" << endl;
    }
    void recover       (istream &is) {
        this->cleanArray();
        value_type f; // It will store the values
        is >> m_vcount >> m_vmax; // Find values from txt
        value_type *pTemp = new value_type[m_vmax]; // Temporary pointer
        for (size_t i = 0; i < m_vcount; ++i) {
            is >> f;   // It stores the values from txt
            pTemp[i] = f; // Save value in temporary pointer
        }
        m_pVect = pTemp; // Save temporary values
    }
    size_t size()
    {  return m_vcount;    }
    value_type &operator[](size_t pos)
    {   return m_pVect[pos];    }
};

template <typename T>
void CArray<T>::resize(){
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

template <typename T>
ostream &operator<<(ostream &os, CArray<T> &obj){
    obj.print(os);
    return os;
}

// TODO
template <typename T>
istream & operator>>(istream &is, CArray<T> &obj){
    // TODO
    obj.recover(is);
    return is;
}


#endif // __ARRAY_H__
