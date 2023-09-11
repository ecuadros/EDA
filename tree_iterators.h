#ifndef __TREE_ITERATOR_H__
#define __TREE_ITERATOR_H__
// #include <unistd.h>
//usleep(microseconds);

template <typename Container>
class inorder_iterator{
    
  public: 
    typedef typename Container::Node           Node; //
    //typedef typename Node::Type         Type;
    typedef inorder_iterator<Container>  myself;

  private:

    Container *m_pContainer;
    Node      *m_pNode;
    bool m_printed;

  public:

    inorder_iterator(Container *pContainer, Node *pNode, bool printed)
        : m_pContainer(pContainer), m_pNode(pNode), m_printed(printed) {}
    inorder_iterator(myself &other) 
          //: m_pContainer(other.m_pContainer), m_pNode(other.m_pNode){}
          : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode), m_printed(other.m_printed){}//ˈʌð.ɚ.waɪz aɪ n't g ˈvæl.juː
    inorder_iterator(myself &&other) // Move constructor
          {   m_pContainer = move(other.m_pContainer);
              m_pNode      = move(other.m_pNode);
          }
    myself operator=(myself &iter)
          {   m_pContainer = move(iter.m_pContainer);
              m_pNode      = move(iter.m_pNode);
              return *(myself *)this; // Pending static_cast?
          }

  public:

    bool operator==(myself iter)   { return m_pNode == iter.m_pNode; }
    bool operator!=(myself iter)   { return !(*this == iter);        }

    Node &operator*(){
      return *m_pNode;
    }

    // myself operator++(int a) {
    //     myself temp(*this);
    //     ++(*this);
    //     return temp;
    // }

    myself operator++() {
        bool r = 0;
        Node* pNode = m_pNode;
        pNode->setVisited(m_printed?0:1);
        while(r == 0){
            pNode = inorder(pNode,r);
        }
        m_pNode = pNode;
        return *this;
    }

    Node* inorder(Node* pNode, bool& r){
        if(pNode->getChild(0) != nullptr && pNode->getChild(0)->getVisited() == m_printed){return inorder(pNode->getChild(0),r);}
        if(pNode->getVisited() == m_printed){r = 1;pNode->setVisited(m_printed?0:1);return pNode;}
        if(pNode->getChild(1) != nullptr && pNode->getChild(1)->getVisited() == m_printed){return inorder(pNode->getChild(1),r);}
        return pNode->getParent();
    }

};

template <typename Container>
class postorden_iterator{
    
  public: 
    typedef typename Container::Node           Node;
    typedef postorden_iterator<Container>  myself;

  private:

    Container *m_pContainer;
    Node      *m_pNode;
    bool m_printed;

  public:

    postorden_iterator(Container *pContainer, Node *pNode, bool printed)
        : m_pContainer(pContainer), m_pNode(pNode), m_printed(printed) {}
    postorden_iterator(myself &other) 
          : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode), m_printed(other.m_printed){}
    postorden_iterator(myself &&other) // Move constructor
          {   m_pContainer = move(other.m_pContainer);
              m_pNode      = move(other.m_pNode);
          }
    myself operator=(myself &iter)
          {   m_pContainer = move(iter.m_pContainer);
              m_pNode      = move(iter.m_pNode);
              return *(myself *)this; // Pending static_cast?
          }

  public:

    bool operator==(myself iter)   { return m_pNode == iter.m_pNode; }
    bool operator!=(myself iter)   { return !(*this == iter);        }

    Node &operator*(){
      return *m_pNode;
    }

    myself operator++() {
        bool r = 0;
        Node* pNode = m_pNode;
        pNode->setVisited(m_printed?0:1);
        while(r == 0){
            pNode = postorden(pNode,r);
        }
        m_pNode = pNode;
        return *this;
    }

    Node* postorden(Node* pNode, bool& r){
        if(pNode->getChild(0) != nullptr && pNode->getChild(0)->getVisited() == m_printed){return postorden(pNode->getChild(0),r);}
        if(pNode->getChild(1) != nullptr && pNode->getChild(1)->getVisited() == m_printed){return postorden(pNode->getChild(1),r);}
        if(pNode->getVisited() == m_printed){r = 1;pNode->setVisited(m_printed?0:1);return pNode;}
        return pNode->getParent();
    }

};

template <typename Container>
class preorden_iterator{
    
  public: 
    typedef typename Container::Node           Node;
    typedef preorden_iterator<Container>  myself;

  private:

    Container *m_pContainer;
    Node      *m_pNode;
    bool m_printed;

  public:

    preorden_iterator(Container *pContainer, Node *pNode, bool printed)
        : m_pContainer(pContainer), m_pNode(pNode), m_printed(printed) {}
    preorden_iterator(myself &other) 
          : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode), m_printed(other.m_printed){}
    preorden_iterator(myself &&other) // Move constructor
          {   m_pContainer = move(other.m_pContainer);
              m_pNode      = move(other.m_pNode);
          }
    myself operator=(myself &iter)
          {   m_pContainer = move(iter.m_pContainer);
              m_pNode      = move(iter.m_pNode);
              return *(myself *)this; // Pending static_cast?
          }

  public:

    bool operator==(myself iter)   { return m_pNode == iter.m_pNode; }
    bool operator!=(myself iter)   { return !(*this == iter);        }

    Node &operator*(){
      return *m_pNode;
    }

    myself operator++() {
        bool r = 0;
        Node* pNode = m_pNode;
        pNode->setVisited(m_printed?0:1);
        while(r == 0){
            pNode = preorden(pNode,r);
        }
        m_pNode = pNode;
        return *this;
    }

    Node* preorden(Node* pNode, bool& r){
        if(pNode->getVisited() == m_printed){r = 1;pNode->setVisited(m_printed?0:1);return pNode;}
        if(pNode->getChild(0) != nullptr && pNode->getChild(0)->getVisited() == m_printed){return preorden(pNode->getChild(0),r);}
        if(pNode->getChild(1) != nullptr && pNode->getChild(1)->getVisited() == m_printed){return preorden(pNode->getChild(1),r);}
        return pNode->getParent();
    }

};

template <typename Container>
class print_iterator{
    
  public: 
    typedef typename Container::Node           Node;
    typedef print_iterator<Container>  myself;

  private:

    Container *m_pContainer;
    Node      *m_pNode;
    bool m_printed;

  public:

    print_iterator(Container *pContainer, Node *pNode, bool printed)
        : m_pContainer(pContainer), m_pNode(pNode), m_printed(printed) {}
    print_iterator(myself &other) 
          : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode), m_printed(other.m_printed){}
    print_iterator(myself &&other) // Move constructor
          {   m_pContainer = move(other.m_pContainer);
              m_pNode      = move(other.m_pNode);
          }
    myself operator=(myself &iter)
          {   m_pContainer = move(iter.m_pContainer);
              m_pNode      = move(iter.m_pNode);
              return *(myself *)this; // Pending static_cast?
          }

  public:

    bool operator==(myself iter)   { return m_pNode == iter.m_pNode; }
    bool operator!=(myself iter)   { return !(*this == iter);        }

    Node &operator*(){
      return *m_pNode;
    }

    myself operator++() {
        bool r = 0;
        Node* pNode = m_pNode;
        pNode->setVisited(m_printed?0:1);
        while(r == 0){
            pNode = printAsTree(pNode,r);
        }
        m_pNode = pNode;
        return *this;
    }

    Node* printAsTree(Node* pNode, bool& r){
        if(pNode->getChild(1) != nullptr && pNode->getChild(1)->getVisited() == m_printed){return printAsTree(pNode->getChild(1),r);}
        if(pNode->getVisited() == m_printed){r = 1;pNode->setVisited(m_printed?0:1);return pNode;}
        if(pNode->getChild(0) != nullptr && pNode->getChild(0)->getVisited() == m_printed){return printAsTree(pNode->getChild(0),r);}
        return pNode->getParent();
    }

};


#endif
 