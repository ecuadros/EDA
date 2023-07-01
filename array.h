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
    void Get          (ifstream &os , CArray &obj);
    
};

ostream & operator<<(ostream &os, CArray &obj);
ifstream & operator>>(ifstream &is, CArray &obj);


#endif // __ARRAY_H__
