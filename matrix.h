#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <iostream>
#include "iterator.h"
#include <cassert>

template <typename Container>
class matrix_iterator 
     : public general_iterator<Container,  class matrix_iterator<Container> > // 
{public: 
    // TODO: subir al padre  
    typedef class general_iterator<Container, matrix_iterator<Container> > Parent; 
    typedef typename Container::Node           Node; // 
    typedef matrix_iterator<Container>  myself;

  public:
    matrix_iterator(Container *pContainer, Node *pNode) 
            : Parent (pContainer,pNode) {}
    matrix_iterator(myself &other)  : Parent (other) {}
    matrix_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

public:
    matrix_iterator operator++() { // Parent::m_pNode--;
                                          return *this;
                                 }
};

template <typename T>
class NodeMatrix
{
public:
  using value_type   = T;
  using Type      = T;
//   using LinkedValueType = V;
private:
  using Node      = NodeMatrix<T> ;
public:
    value_type       m_key;
    // LinkedValueType      m_value;

public:
    NodeMatrix(value_type key) 
        : m_key(key) {}

    NodeMatrix(const NodeMatrix<T>& other) : 
        NodeMatrix(other.m_key) {}

    NodeMatrix(NodeMatrix<T>&& other) // Move constructor
        : m_key  (std::move(other.m_key)) {}
    NodeMatrix() {}

    NodeMatrix& operator=(const NodeMatrix& other) {
        if (this != &other) {
            m_key = other.m_key;
        }
        return *this;
    }

    value_type    getData() const   { return m_key; }
    value_type&   getDataRef()      { return m_key; }

    bool operator<(const NodeMatrix<T>& other) const { 
        return m_key < other.m_key;
    }
    // Error was here. Next line was missing
    bool operator>(const NodeMatrix<T>& other) const { 
        return m_key > other.m_key;
    }
    
};

template <typename _K>
struct MatrixTrait
{
    using  value_type      = _K;
    using  Node      = NodeMatrix<_K>;
    // using  CompareFn = _CompareFn;
};

using MatrixTraitFloat = MatrixTrait<TX>;

template <typename Traits>
class CMatrix
{public:
    using value_type      = typename Traits::value_type;
    using Node            = typename Traits::Node;
    //using CompareFn       = typename Traits::CompareFn;
    using myself          = CMatrix<Traits>;
    //using iterator        = matrix_iterator<myself>;

    using nodeMatrix          = NodeMatrix<value_type>;

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

    void create(size_t rows, size_t cols){
        destroy();
        m_rows = rows;
        m_cols = cols;
        m_ppMatrix = new Node *[m_rows];
        for(auto i = 0 ; i < m_rows ; i++)
            m_ppMatrix[i] = new Node[m_cols];
            // *(res+i) = new TX[m_cols];
            // *(i+res) = new TX[m_cols];
            // i[res]   = new TX[m_cols];
        
    }
    
    void fill(value_type val){
        for(auto y = 0 ; y < m_rows ; y++)
            for(auto x = 0 ; x < m_cols ; x++)
                m_ppMatrix[y][x] = val;
                // *(m_ppMatrix+y)[x] = val;
                // *(*(m_ppMatrix+y)+x) = val;
                // *(y[m_ppMatrix]+x) = val;
                // x[y[m_ppMatrix]] = val;
    }

    void print(ostream &os){
        os << m_rows << " " << m_cols << endl;
        for(auto y = 0 ; y < m_rows ; y++){
            for(auto x = 0 ; x < m_cols ; x++)
                os << m_ppMatrix[y][x].getDataRef() << " ";
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
    
    value_type &operator()(size_t rows, size_t cols){
        assert( rows < m_rows && cols < m_cols );
        return m_ppMatrix[rows][cols].getDataRef();
    }
    Node* &operator[](size_t rows){
        assert( rows < m_rows );
        return m_ppMatrix[rows];
    }

    // iterator begin() { iterator iter(this, m_ppMatrix);    return iter;    }
    // iterator end()   { iterator iter(this, m_pVect+m_vcount);    return iter;    }

};

template <typename Traits>
ostream &operator<<(ostream &os, CMatrix<Traits> &mat){
    mat.print(os);
    return os;
}

#endif