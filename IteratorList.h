
//SE REMOVIÓ GENERAL ITERATOR

template <typename Container>

class LinkedList_iterator  {
public:
    typedef typename Container::Node    Node;
    typedef LinkedList_iterator<Container> myself;


protected:
    Container *m_pContainer;
    Node      *m_pNode;

public:
    LinkedList_iterator(Container *pContainer, Node *pNode): m_pContainer(pContainer), m_pNode(pNode) {}
    LinkedList_iterator(myself &other): m_pContainer(other.m_pContainer), m_pNode(other.m_pNode){}
    LinkedList_iterator(myself &&other) {  // Move constructor
                 m_pContainer = move(other.m_pContainer);
                m_pNode      = move(other.m_pNode);
            }
    myself operator=(myself &iter){  
              m_pContainer = move(iter.m_pContainer);
              m_pNode      = move(iter.m_pNode);
              return *(myself *)this; 
          }

    bool operator==(myself iter)   { return m_pNode == iter.m_pNode; }
    bool operator!=(myself iter)   { return !(*this == iter);        }
    //...............................................................
  
    Node &operator*(){return *m_pNode; }
    
    myself operator++() { 
          Node*  temp=m_pNode->getpNext();
          m_pNode=temp;
          
          return *this; //  devuelve  una referencia al objeto actual, no al puntero al objeto.
          }
};