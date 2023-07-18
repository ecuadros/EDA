#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <iostream>
#include "iterator.h"
using namespace std;

template<typename Container>
class matrix_iter{
    public:
        typedef typename Container::value_type value_type;
        //typedef typename Node::Type      Type;
        typedef matrix_iter<Container> myself;
    protected:
        Container  *m_pContainer;
        value_type *m_pNode;
        TX        m_dirIter;
        size_t    n_rows=0; 
        size_t    n_cols=0;
        size_t    mp_rows;// Rows of Matrix
        size_t    mp_cols; // Columns of Matrix   
    public:
        matrix_iter(Container *pContainer, value_type *pNode,TX dirIter)
        : m_pContainer(pContainer), m_pNode(pNode),m_dirIter(dirIter){
            mp_rows= m_pContainer->m_rows;
            mp_cols= m_pContainer->m_cols;
        }
        matrix_iter(myself &other,TX dirIter)
        : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode),m_dirIter(dirIter){
            mp_rows= m_pContainer->m_rows;
            mp_cols= m_pContainer->m_cols;
        }
        matrix_iter(myself &&other,TX dirIter) {   
            m_pContainer = move(other.m_pContainer);
            m_pNode      = move(other.m_pNode);
            m_dirIter= dirIter;
            mp_rows= m_pContainer->m_rows;
            mp_cols= m_pContainer->m_cols;
          }
        bool operator==(myself iter)   { return m_pNode == iter.m_pNode; }
        bool operator!=(myself iter)   { return !(*this == iter);}
        
        value_type &operator*(){
             return m_pContainer->getDataRef(n_rows,n_cols);
             }
        matrix_iter &operator++() {
            if (m_dirIter == 1){
                if (n_rows<=mp_rows-1){
                    if(n_cols<mp_cols-1){
                       n_cols++;
                       m_pNode=m_pContainer->m_ppMatrix[n_rows]+n_cols;
                    }
                    else{
                      if(n_rows<=mp_rows-1){
                        n_rows++;  
                        n_cols=0;
                        m_pNode=m_pContainer->m_ppMatrix[n_rows];
                      }   
                    }
                }
                return *this;
            }
            else{
                 //m_pNode--;
                return *this;
            }
        }
};

// template <typename Container>
// class matrix_iterator 
//      : public general_iterator<Container,  class matrix_iterator<Container> > // 
// {public: 
//     // TODO: subir al padre  
//     typedef class general_iterator<Container, matrix_iterator<Container> > Parent; 
//     typedef typename Container::Node           Node; // 
//     typedef matrix_iterator<Container>  myself;

//   public:
//     array_backward_iterator(Container *pContainer, Node *pNode) 
//             : Parent (pContainer,pNode) {}
//     array_backward_iterator(myself &other)  : Parent (other) {}
//     array_backward_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

// public:
//     matrix_iterator operator++() { // Parent::m_pNode--;
//                                           return *this;
//                                  }
// };

template <typename _K>
struct MatrixTrait
{
    using  value_type      = _K;
    // using  LinkedValueType = _V;
    // using  Node      = NodeArray<_K, _V>;
    // using  CompareFn = _CompareFn;
};

using MatrixTraitFloat = MatrixTrait<float>;
using MatrixTraitInt= MatrixTrait<int>;

template <typename Traits>
class CMatrix{
    public:
        using value_type      = typename Traits::value_type;
        //using LinkedValueType = typename Traits::LinkedValueType;
        //using Node            = typename Traits::Node;
        //using CompareFn       = typename Traits::CompareFn;
        using myself          = CMatrix<Traits>;
        using iterator        = matrix_iter<myself>;
    public:
        value_type **m_ppMatrix   = nullptr;
        size_t m_rows = 0, m_cols = 0;
    public:
        CMatrix(size_t rows, size_t cols){   
            create(rows, cols); 
        }
        ~CMatrix(){
            destroy();
        }
        void Printdir(){
            cout<<m_ppMatrix[0][0]<<endl;
            cout<<m_ppMatrix[m_rows-1][m_cols-1]<<endl;
            cout<<m_ppMatrix[0]<<endl;
            cout<<m_ppMatrix[0]+1<<endl;
            cout<<*(m_ppMatrix[0]+1)<<endl;
            cout<<*(m_ppMatrix[m_rows-1]+m_cols-1)<<endl;
            cout<<m_ppMatrix<<endl;
        }
        void create(size_t rows, size_t cols){
            destroy();
            m_rows = rows;
            m_cols = cols;
            m_ppMatrix = new value_type *[m_rows];
            for(auto i = 0 ; i < m_rows ; i++)
                m_ppMatrix[i] = new value_type[m_cols];
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
        value_type&   getDataRef(size_t row, size_t col){ return m_ppMatrix[row][col]; }
        iterator begin() { iterator iter(this, m_ppMatrix[0],1);    return iter;    }
        iterator end()   { iterator iter(this, m_ppMatrix[m_rows-1]+m_cols-1,1);    return iter;    }

        value_type* operator[](size_t index) {
        return m_ppMatrix[index];
    }

};

template <typename Traits>
ostream &operator<<(ostream &os, CMatrix<Traits> &mat){
    mat.print(os);
    return os;
}

#endif