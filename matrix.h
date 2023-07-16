#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <iostream>

template <typename _K>
struct MatrixTrait
{
    using  value_type      = _K;
    // using  LinkedValueType = _V;
    // using  Node      = NodeArray<_K, _V>;
    // using  CompareFn = _CompareFn;
};

using MatrixTraitFloat = MatrixTrait<float>;

template <typename Traits>
class CMatrix
{public:
    using value_type      = typename Traits::value_type;
    //using LinkedValueType = typename Traits::LinkedValueType;
    //using Node            = typename Traits::Node;
    //using CompareFn       = typename Traits::CompareFn;
    using myself          = CMatrix<Traits>;
    //using iterator        = matrix_forward_iterator<myself>;

    private:
        value_type **m_ppMatrix   = nullptr;
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
    void fill_index(value_type val,size_t i,size_t j){
        m_ppMatrix[i][j] = val;
    }
    auto get_value(size_t i,size_t j){
        return m_ppMatrix[i][j]; 
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
    CMatrix<Traits> operator*(CMatrix<Traits> &other){
        CMatrix<Traits> res(m_rows, other.m_cols);
        if (m_rows==other.m_rows && m_cols==other.m_cols){
            for (int i = 0; i < m_rows; i++) {
                for (int j = 0; j < other.m_cols; j++) {
                    //res.fill_index(m_ppMatrix[i][j]*other.get_value(i,j),i,j);
                    //res[i][j]=m_ppMatrix[i][j]*other[i][j];
                    res[i][j]=(*this)[i][j]*other[i][j];
                }
            }
        }
        return res;
    }
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