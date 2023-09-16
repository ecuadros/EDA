
//SE REMOVIÓ GENERAL ITERATOR

//---------------------------------------------------------------------------------


template <typename Container>

class Double_backward_LinkedList_iterator  {
public:
	
    typedef typename Container::Node    						Node;
    typedef Double_backward_LinkedList_iterator<Container> 		myself;

protected:
    Container	 	*m_pContainer;
    Node      		*m_pNode;

public:
	
    Double_backward_LinkedList_iterator(Container *pContainer, Node *pNode):	 m_pContainer(pContainer), m_pNode(pNode) {}
    
    Double_backward_LinkedList_iterator(myself &other):						 m_pContainer(other.m_pContainer), m_pNode(other.m_pNode){}
    
    Double_backward_LinkedList_iterator(myself &&other) {

                 m_pContainer 	= move(other.m_pContainer);
                 m_pNode      	= move(other.m_pNode);
                
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
          Node*  temp=m_pNode->getpPrev();
          m_pNode=temp;
          
          return *this; 
          }
};







