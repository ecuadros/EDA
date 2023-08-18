#ifndef __TREE_ITERATOR_H__
#define __TREE_ITERATOR_H__

template <typename Container>
class binary_tree_inorder_iterator{

  public: 
    typedef typename Container::Node           Node;
    typedef binary_tree_inorder_iterator<Container>  myself;

  private:

    Container *m_pContainer;
    Node      *m_pNode;
    bool m_printed;

  public:

    binary_tree_inorder_iterator(Container *pContainer, Node *pNode, bool printed)
        : m_pContainer(pContainer), m_pNode(pNode), m_printed(printed) {}
    binary_tree_inorder_iterator(const myself &other) 
          : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode), m_printed(other.m_printed){}
    binary_tree_inorder_iterator(myself &&other) // Move constructor
          : m_pContainer(std::move(other.m_pContainer)), m_pNode(std::move(other.m_pNode)), m_printed(other.m_printed){}
    myself& operator=(const myself &iter)
          {   m_pContainer = iter.m_pContainer;
              m_pNode      = iter.m_pNode;
              m_printed    = iter.m_printed;
              return *this;
          }

  public:

    bool operator==(const myself &iter) const { return m_pNode == iter.m_pNode; }
    bool operator!=(const myself &iter) const { return !(*this == iter);        }

    Node &operator*(){
      return *m_pNode;
    }

    myself& operator++() {
        bool r = false;
        Node* pNode = m_pNode;
        pNode->setPrinted(m_printed ? false : true);
        while (!r) {
            pNode = inorder(pNode, r);
        }
        m_pNode = pNode;
        return *this;
    }

    Node* inorder(Node* pNode, bool& r) {
        if (pNode->getChild(0) != nullptr && pNode->getChild(0)->getPrinted() == m_printed) {
            return inorder(pNode->getChild(0), r);
        }
        if (pNode->getPrinted() == m_printed) {
            r = true;
            pNode->setPrinted(m_printed ? false : true);
            return pNode;
        }
        if (pNode->getChild(1) != nullptr && pNode->getChild(1)->getPrinted() == m_printed) {
            return inorder(pNode->getChild(1), r);
        }
        return pNode->getParent();
    }

};

template <typename Container>
class binary_tree_postorden_iterator{

  public: 
    typedef typename Container::Node           Node;
    typedef binary_tree_postorden_iterator<Container>  myself;

  private:

    Container *m_pContainer;
    Node      *m_pNode;
    bool m_printed;

  public:

    binary_tree_postorden_iterator(Container *pContainer, Node *pNode, bool printed)
        : m_pContainer(pContainer), m_pNode(pNode), m_printed(printed) {}
    binary_tree_postorden_iterator(const myself &other) 
          : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode), m_printed(other.m_printed){}
    binary_tree_postorden_iterator(myself &&other) // Move constructor
          : m_pContainer(std::move(other.m_pContainer)), m_pNode(std::move(other.m_pNode)), m_printed(other.m_printed){}
    myself& operator=(const myself &iter)
          {   m_pContainer = iter.m_pContainer;
              m_pNode      = iter.m_pNode;
              m_printed    = iter.m_printed;
              return *this;
          }

  public:

    bool operator==(const myself &iter) const { return m_pNode == iter.m_pNode; }
    bool operator!=(const myself &iter) const { return !(*this == iter);        }

    Node &operator*(){
      return *m_pNode;
    }

    myself& operator++() {
        bool r = false;
        Node* pNode = m_pNode;
        pNode->setPrinted(m_printed ? false : true);
        while (!r) {
            pNode = postorden(pNode, r);
        }
        m_pNode = pNode;
        return *this;
    }

    Node* postorden(Node* pNode, bool& r) {
        if (pNode->getChild(0) != nullptr && pNode->getChild(0)->getPrinted() == m_printed) {
            return postorden(pNode->getChild(0), r);
        }
        if (pNode->getChild(1) != nullptr && pNode->getChild(1)->getPrinted() == m_printed) {
            return postorden(pNode->getChild(1), r);
        }