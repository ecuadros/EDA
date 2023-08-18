#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <iostream>
#include <cassert>
#include "iterator.h"

template <typename Container>
class matrix_iterator 
     : public general_iterator<Container,  class matrix_iterator<Container> > // 
{public: 
    // TODO: subir al padre  
    typedef class general_iterator<Container, matrix_iterator<Container> > Parent; 
    typedef typename Container::Node           Node; // 
    typedef matrix_iterator<Container>  myself;

    public:

        size_t i = 0;
        size_t j = 0;

  public:
    matrix_iterator(Container *pContainer, Node *pNode) 
            : Parent (pContainer,pNode) {}
    matrix_iterator(myself &other)  : Parent (other) {}
    matrix_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

public:
    matrix_iterator operator++(){
        if(j == Parent::m_pContainer->cols() - 1){ //t
            j = 0;
            i++;
            Parent::m_pNode = Parent::m_pContainer->firstPtrRow(i);
        }else{
            j++;
            Parent::m_pNode++;
        }        
        return *this;
    }
};

template <typename T>
class NodeMatrix
{
public:
  using value_type   = T;
  using Type      = T;
private:
  using myself      = NodeMatrix<T> ;
public:
    value_type       m_key;

public:
    
    NodeMatrix() {}
    
    NodeMatrix(value_type key) 
        : m_key(key) {}

    value_type    getData() const   { return m_key; }
    value_type&   getDataRef()      { return m_key; }

    constexpr operator value_type() const noexcept { //d since C++14
        return m_key;
    }

};

template <typename _K>
struct MatrixTrait
{
    using  value_type   = _K;
    using  Node      = NodeMatrix<_K>;
};

using MatrixTraitFloat = MatrixTrait<float>;

template <typename Traits>
class CMatrix
{public:
    using value_type      = typename Traits::value_type;
    using Node            = typename Traits::Node;
    using myself          = CMatrix<Traits>;
    using iterator        = matrix_iterator<myself>;

    private:
        Node **m_ppMatrix   = nullptr;
        size_t m_rows = 0, m_cols = 0;
public:
    CMatrix(size_t rows, size_t cols)
    {   create(rows, cols); 
    }
    ~CMatrix(){
        destroy();
    }

    CMatrix(myself &&other){        
        m_rows = std::move(other.m_rows);
        m_cols = std::move(other.m_cols);
        m_ppMatrix = std::move(other.m_ppMatrix);
    }

    size_t rows(){
        return m_rows;
    }

    size_t cols(){
        return m_cols;
    }

    void create(size_t rows, size_t cols){
        destroy();
        m_rows = rows;
        m_cols = cols;
        m_ppMatrix = new Node *[m_rows]; //like in array.h
        for(auto i = 0 ; i < m_rows ; i++)
            m_ppMatrix[i] = new Node[m_cols];
            // *(res+i) = new TX[m_cols];
            // *(i+res) = new TX[m_cols];
            // i[res]   = new TX[m_cols];
    }
    
    void fill(value_type val){
        for(auto y = 0 ; y < m_rows ; y++)
            for(auto x = 0 ; x < m_cols ; x++)
                m_ppMatrix[y][x] = Node(val);
                //m_ppMatrix[y][x] = val;
                // *(m_ppMatrix+y)[x] = val;
                // *(*(m_ppMatrix+y)+x) = val;
                // *(y[m_ppMatrix]+x) = val;
                // x[y[m_ppMatrix]] = val;
    }

    void print(ostream &os){
        os << m_rows << " " << m_cols << endl;
        for(auto y = 0 ; y < m_rows ; y++){
            for(auto x = 0 ; x < m_cols ; x++)
                //os << m_ppMatrix[y][x] << " ";
                os << m_ppMatrix[y][x] << " ";
            os << endl;
        }
    }
    void destroy(){
        for(auto y = 0 ; y < m_rows ; y++)
            delete [] m_ppMatrix[y];
        delete [] m_ppMatrix;
        m_ppMatrix = nullptr;
        m_rows = m_cols = 0;
    }

    // CMatrix<Traits> operator*(const CMatrix<Traits> &other){
    //     CMatrix<Traits> res(m_rows, other.m_cols);
        
    //     return res;
    // }
    
    iterator begin() { iterator iter(this, m_ppMatrix[0]);    return iter;    }
    iterator end()   { iterator iter(this, m_ppMatrix[m_rows]);    return iter;    }

    Node* firstPtrRow(size_t row){
        return m_ppMatrix[row];
    }

};

template <typename Traits>
ostream &operator<<(ostream &os, CMatrix<Traits> &mat){
    mat.print(os);
    return os;
}

#endif