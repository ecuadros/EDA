#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <iostream>
using namespace std;

class CArray{
private:
    int *m_pVect = nullptr;
    int  m_vcount = 0, m_vmax = 0;
    string m_name = "Empty";
public:
    CArray();
    CArray(string name);   // Constructor
    ~CArray();
    void insert       (int val);
    void resize       ();
    void print        (ostream &os);
    void read         (istream &is);
    void fill         (istream &is);
    int size()
    {  return m_vcount;    }
    int &operator[](int pos)
    {   return m_pVect[pos];    }
};

ostream & operator<<(ostream &os, CArray &obj);

istream &operator>>(istream &is, CArray &obj);

#endif // __ARRAY_H__
