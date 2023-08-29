#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <iostream>
#include <cassert>
#include <cstddef>
using namespace std;
//========= Adding Iterator ================================
//==========================================================
template <typename Container>
class M_iterator 
{public: 
   
    typedef typename Container::Node    Node; 
    typedef typename Node::Type         Type;
    typedef M_iterator<Container>  myself;
public:
    size_t count_row=0;
    size_t count_col=0;

protected:
    Container *m_pContainer;
    Node *m_pNode;
   

  public:
    M_iterator(Container *pContainer, Node *pNode): m_pContainer(pContainer), m_pNode(pNode) {}
    M_iterator(myself &other)  : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode){}
    M_iterator(myself &&other) { m_pContainer = move(other.m_pContainer); m_pNode = move(other.m_pNode);} 

public:
//==================== For the matrix's iterator ==============
    M_iterator operator++() { 
       if(count_col!=m_pContainer->getColumn()){
        cout << "----------> ("<<count_row<<" , "<<count_col<<")"<<endl;
        count_col++;
        m_pNode=m_pContainer->getPtr(count_row)+count_col;
       }
       else{
           cout<<"---------------------"<<endl;           
            count_col=0;
           ++count_row;
            m_pNode=m_pContainer->getPtr(count_row);
           
           
       }
     return *this;
    }

                                                          
    M_iterator operator=(M_iterator &iter)
          {   m_pContainer = move(iter.m_pContainer);
              m_pNode      = move(iter.m_pNode);
              return *(M_iterator*)this; 
          }

    bool operator==(M_iterator iter)   { return m_pNode == iter.m_pNode; }
    bool operator!=(M_iterator iter)   { return !(*this == iter);        }
    Type &operator*()                    { return m_pNode->getDataRef();   }                                  
};

//============================================
//============================================
template <typename T>
class NodeMatrix
{
public:
  using value_type   = T;
  using Type      = T;
  value_type       m_key;
private:
  using myself      = NodeMatrix<T> ;

public:
    
    NodeMatrix() {}
    
    NodeMatrix(value_type key) : m_key(key) {}

    value_type    getData() const   { return m_key; }
    value_type&   getDataRef()      { return m_key; }

    value_type operator+=(const value_type value) {
        m_key += value;
        return m_key;
    }

    constexpr operator value_type() const noexcept { 
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
    using iterator        = M_iterator<myself>;

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

    myself operator*(myself &other){
        assert(m_cols == other.m_rows);
        myself answer(m_rows,other.m_cols);
        myself &me = *this;
        for(auto row = 0; row < m_rows; row++){
            for(auto col = 0; col < other.m_cols; col++){
                answer[row][col] = 0 ; //elimino valores proximos al 0
                for(auto i = 0 ; i < m_cols ; i++){
                    answer[row][col] +=  me[row][i] * other[i][col];
                }
            }
        }
        return answer;
    }
    //Operator ValueType
     value_type &operator()(size_t rows, size_t cols){
       // cout<<"numero de filas "<<m_rows<<endl;
        assert(rows<m_rows && cols<m_cols);
        return m_ppMatrix[rows][cols].getDataRef();

    }
    
    Node* operator[](size_t row){
        
        assert( row < m_rows );
        return m_ppMatrix[row];
    }

 //================== ITERATORS ==============================
 //===========================================================
    size_t getRow(){return m_rows;}
    size_t getColumn(){return m_cols;}
    Node* getPtr(size_t row) {return m_ppMatrix[row];}
    iterator begin() {  
        iterator iter(this, m_ppMatrix[0]);    
        return iter;    }
    iterator end()   {  
        iterator  iter(this,  m_ppMatrix [m_rows]);   
         return iter;    }

};

template <typename Traits>
ostream &operator<<(ostream &os, CMatrix<Traits> &mat){
    mat.print(os);
    return os;
}

#endif