#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <iostream>
using namespace std;

class CArray{
private:
    //typedef typename Traits::T          value_type;
    typedef int value_type;
    value_type *m_pVect = nullptr;
    size_t  m_vcount = 0, m_vmax = 0;
    string m_name = "Empty";
public:
    CArray();
    CArray(string name);   // Constructor
    ~CArray();
    void insert       (value_type val);
    void resize       ();
    void print        (ostream &os);
    size_t size()
    {  return m_vcount;    }
    value_type &operator[](size_t pos)
    {   return m_pVect[pos];    }
};

ostream & operator<<(ostream &os, CArray &obj);

// TODO
istream & operator<<(istream &is, CArray &obj);


#endif // __ARRAY_H__
