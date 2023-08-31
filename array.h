#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <iostream>
#include <memory>

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
        //cout << "Val=" << val << " inserted, m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
    }
    void resize       ();
    void print        (ostream &os){
        // os << "Printing: " << m_name << endl;
        os << m_vcount << " " << m_vmax << endl;
        for(size_t i = 0; i < m_vcount ; ++i )
            os << m_pVect[i] << endl;
        //os << "m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
    }
    size_t size()
    {  return m_vcount;    }
    value_type &operator[](size_t pos)
    {   return m_pVect[pos];    }

    //=====================================================================================
    void read (istream &is)
    {
        //En caso ya se tenga un objeto CArrray con algún valor, entonces destruirlo.
        if (m_vcount != 0)
        {
            cout<< endl << "***El objeto " << m_name << " no estaba vacio, se ha reescrito***." << endl << endl;
            delete [] m_pVect;
            m_pVect = nullptr;
            m_vcount = 0;
            m_vmax = 0;
        }

        //Los *dos* primeros valores que se almacenan en el txt son enteros, siendo el
        //primero la cantidad de elementos y el segundo la capacidad máxima, por ello:
        is >> m_vcount;
        is >> m_vmax;

        //Los valores siguientes, debido al Template (value_type) pueden ser int, char,
        //string, bool, etc. Por ello, los siguientes valores a capturar han de ser del
        //tipo value_type llamado elemento.
        value_type elemento;
        
        //Se salva la cantidad de elementos para poder hacer la misma cantidad de lectura
        //de elementos del texto.
        int numElementos = m_vcount;
        
        //Como se ha de emplear la función insert, este se encargará del cálculo de la
        //cantidad de elementos y tamaño del Array, por eso regresamos los valores a 0.
        m_vcount = 0; m_vmax = 0;
        for (size_t i = 0; i < numElementos; i++)
        {
            is >> elemento;
            insert(elemento);
        }
    }


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
istream & operator>>(istream &is, CArray<T> &obj)
{
    obj.read(is);
    return is;
}


#endif // __ARRAY_H__
