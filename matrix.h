#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <iostream>
#include <cassert>
#include "keynode.h"

template <typename Container>
class matrix_iterator 
     : public general_iterator<Container,  class matrix_iterator<Container> > // 
{
public: 
    typedef class general_iterator<Container, matrix_iterator<Container> > Parent; 
    typedef typename Container::Node           Node; // 
    typedef matrix_iterator<Container>  myself;
public:
    size_t i = 0;
    size_t j = 0;
public:
    matrix_iterator(Container *pContainer, Node *pNode) : Parent (pContainer,pNode) {}
    matrix_iterator(myself &other)  : Parent (other) {}
    matrix_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

public:
    matrix_iterator operator++(){
        j++;
        if(j >= Parent::m_pContainer->m_cols){
            j = 0;
            i++;
        }
        if(i < Parent::m_pContainer->m_rows){
            Parent::m_pNode = &((*Parent::m_pContainer)[i][j]);
        }
        else {
            Parent::m_pNode = nullptr;
        }
        return *this;
    }
};



template <typename T>
class NodeMatrix
{
public:
  using value_type   = typename T::value_type;
  using InnerNode = typename T::InnerNode;
  using Type = typename T::value_type;
    using LinkedValueType = typename T::LinkedValueType;
private:
  using myself      = NodeMatrix<T> ;
public:
    InnerNode      m_data;

public:
    
    NodeMatrix() {}
    
    NodeMatrix(value_type key, LinkedValueType value) 
        : m_data(key, value) {}

    value_type    getData() const   { return m_data.m_key; }
    value_type&   getDataRef()      { return m_data.m_key; }

    value_type operator+=(const value_type value) {//c
        m_data.m_key += value;
        return m_data.m_key;
    }

    constexpr operator value_type() const noexcept { //d since C++14
        return m_data.m_key;
    }

};

template <typename _K, typename _V>
struct MatrixTrait
{
    using value_type   = _K;
    using LinkedValueType = _V;
    using Node      = NodeMatrix<MatrixTrait<_K, _V>>;
    using InnerNode = KeyNode<_K, _V>;
    //using  CompareFn = _CompareFn;
};

using MatrixTraitFloatFloat = MatrixTrait<float, float>;

template <typename Traits>
class CMatrix
{public:
    using value_type      = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using Node            = typename Traits::Node;
    using myself          = CMatrix<Traits>;
    using iterator        = matrix_iterator<myself>;

    private:
        Node **m_ppMatrix   = nullptr;
        size_t m_rows = 0, m_cols = 0;
public:
    friend class matrix_iterator<myself>;
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
    
    void fill(value_type val, LinkedValueType val2){
        for(auto y = 0 ; y < m_rows ; y++)
            for(auto x = 0 ; x < m_cols ; x++)
                m_ppMatrix[y][x] = Node(val, val2);
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

    myself operator*(myself &other){
        assert(m_cols == other.m_rows);
        myself answer(m_rows,other.m_cols);
        myself &me = *this;
        for(auto row = 0; row < m_rows; row++){
            for(auto col = 0; col < other.m_cols; col++){
                answer[row][col].getDataRef() = 0 ;
                for(auto i = 0 ; i < m_cols ; i++){
                    answer[row][col].getDataRef() +=  me[row][i].getDataRef() * other[i][col].getDataRef();
                }
            }
        }
        return answer;
    }
    
    value_type &operator()(size_t row, size_t col){
        assert( row < m_rows && col < m_cols );
        return m_ppMatrix[row][col].getDataRef();
    }
    
    Node* operator[](size_t row){
        assert( row < m_rows );
        return m_ppMatrix[row];
    } 
    Node* firstPtrRow(size_t row){
        return m_ppMatrix[row];
    }

    iterator begin() { iterator iter(this, m_ppMatrix[0]);    return iter;    }
    iterator end()   { iterator iter(this, m_ppMatrix[m_rows]);    return iter;    }

};

template <typename Traits>
ostream &operator<<(ostream &os, CMatrix<Traits> &mat){
    mat.print(os);
    return os;
}

#endif