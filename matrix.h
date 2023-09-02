#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <iostream>

template <typename _K>
struct MatrixTrait
{
    using  value_type      = _K;
};

using MatrixTraitFloat = MatrixTrait<float>;

template <typename Traits>
class CMatrix
{public:
    using value_type      = typename Traits::value_type;
    using myself          = CMatrix<Traits>;


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
        
    }
    
    void fill(value_type val){
        for(auto y = 0 ; y < m_rows ; y++)
            for(auto x = 0 ; x < m_cols ; x++)
                m_ppMatrix[y][x] = val;
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

    CMatrix<Traits> multiply(CMatrix<Traits> &other)
    {
        CMatrix<Traits> producto(m_rows, other.m_cols);
        // Se inicializa la matriz producto con 0, para evitar tomar valores ya guardados
        // en esos espacios de memoria.
        producto.fill(0);
        try
        {
            if(m_cols != other.m_rows)
                throw "El multiplicando y multiplicador no tienen dimensiones validas";


            // Una vez se ha asegurado que la multiplicaciÃ³n matricial es
            // vÃ¡lida se completa el producto.

            for(auto i = 0; i< m_rows; i++)
                for(auto j= 0; j<other.m_cols; j++)
                    for(auto k = 0; k < m_cols; k++)
                        producto.m_ppMatrix[i][j] += m_ppMatrix[i][k] * other.m_ppMatrix[k][j];
            return producto;
        }
        catch (const char* msg)
        {
            cerr << msg << endl;
            return producto;
        }
    }

    CMatrix<Traits> operator *(CMatrix<Traits> &other)
    {
        return multiply(other);
    }
};

template <typename Traits>
ostream &operator<<(ostream &os, CMatrix<Traits> &mat){
    mat.print(os);
    return os;
}

#endif