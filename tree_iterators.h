#ifndef __TREE_ITERATOR_H__
#define __TREE_ITERATOR_H__
// #include <unistd.h>
// usleep(microseconds);

// Esta clase hará el recorrido del iterador sobre el arbol binario
// en orden del menor al mayor valor.
template <typename Container>
class binary_tree_inorder_iterator
{

public:
  typedef typename Container::Node Node; //
  // typedef typename Node::Type         Type;
  typedef binary_tree_inorder_iterator<Container> myself;

private:
  Container *m_pContainer;
  Node *m_pNode;
  bool m_printed;

public:

  // Se declaran los constructores de la clase
  binary_tree_inorder_iterator(Container *pContainer, Node *pNode, bool printed)
  {
    m_pContainer = pContainer;
    m_pNode = pNode;
    m_printed = printed;
  }


public:
  bool operator==(myself iter)
  {
    return m_pNode == iter.m_pNode;
  }

  bool operator!=(myself iter)
  {
    return m_pNode != iter.m_pNode;
  }

  Node &operator*()
  {
    return *m_pNode;
  }

  myself operator++()
  {
    bool r = 0;
    Node *pNode = m_pNode;

    if (m_printed == true)
      pNode->setPrinted(0);
    else
      pNode->setPrinted(1);

    while (r == 0)
    {
      pNode = inorder(pNode, r);
    }
    m_pNode = pNode;
    return *this;
  }

  Node *inorder(Node *pNode, bool &r)
  {
    // Esta parte garantiza que se encuentre en el nodo más inferior
    // del subarbol izquierdo
    if (pNode->getChild(0) != nullptr && pNode->getChild(0)->getPrinted() == m_printed)
    {
      return inorder(pNode->getChild(0), r);
    }

    // Esto retorna el hijo en el que se encuentre
    if (pNode->getPrinted() == m_printed)
    {
      r = 1;
      pNode->setPrinted(m_printed ? 0 : 1);
      return pNode;
    }

    // Esto garantiza que se visite el hijo derecho una vez agotada
    // la prioridad sobre el hijo izquierdo
    if (pNode->getChild(1) != nullptr && pNode->getChild(1)->getPrinted() == m_printed)
    {
      return inorder(pNode->getChild(1), r);
    }

    // Esto retorna el nodo padre una vez agotado las condiciones anteriores.
    return pNode->getParent();
  }
};

// Esta clase hará el recorrido del iterador sobre el arbol binario
// empezando por el nodo padre, para luego ir dando prioridad al hijo izquierdo,
// en caso no hubiese más hijos izquierdos se visita al hijo derecho.
template <typename Container>
class binary_tree_postorden_iterator
{

public:
  typedef typename Container::Node Node;
  typedef binary_tree_postorden_iterator<Container> myself;

private:
  Container *m_pContainer;
  Node *m_pNode;
  bool m_printed;

public:
  binary_tree_postorden_iterator(Container *pContainer, Node *pNode, bool printed)
  {
    m_pContainer = pContainer;
    m_pNode = pNode;
    m_printed = printed;
  }

public:
  bool operator==(myself iter)
  {
    return m_pNode == iter.m_pNode;
  }

  bool operator!=(myself iter)
  {
    return m_pNode != iter.m_pNode;
  }

  Node &operator*()
  {
    return *m_pNode;
  }

  myself operator++()
  {
    bool r = 0;
    Node *pNode = m_pNode;
    
    if (m_printed == true)
      pNode->setPrinted(0);
    else
      pNode->setPrinted(1);

    while (r == 0)
    {
      pNode = postorden(pNode, r);
    }
    m_pNode = pNode;
    return *this;
  }

  Node *postorden(Node *pNode, bool &r)
  {
    if (pNode->getChild(0) != nullptr && pNode->getChild(0)->getPrinted() == m_printed)
    {
      return postorden(pNode->getChild(0), r);
    }

    if (pNode->getChild(1) != nullptr && pNode->getChild(1)->getPrinted() == m_printed)
    {
      return postorden(pNode->getChild(1), r);
    }

    if (pNode->getPrinted() == m_printed)
    {
      r = 1;
      pNode->setPrinted(m_printed ? 0 : 1);
      return pNode;
    }

    return pNode->getParent();
  }
};

// Esta clase hara el recorrido del iterador sobre el arbol binario
// iniciando en el subarbol izquierdo más bajo y dando prioridad al hijo izquierdo
// sobre el hijo derecho. Luego se sube al padre y se hace el mismo procedimiento.
// Dejando para el final al padre del arbol.
template <typename Container>
class binary_tree_preorden_iterator
{

public:
  typedef typename Container::Node Node;
  typedef binary_tree_preorden_iterator<Container> myself;

private:
  Container *m_pContainer;
  Node *m_pNode;
  bool m_printed;

public:
  binary_tree_preorden_iterator(Container *pContainer, Node *pNode, bool printed)
  {
    m_pContainer = pContainer;
    m_pNode = pNode;
    m_printed = printed;
  }

public:
  bool operator==(myself iter) 
  { 
    return m_pNode == iter.m_pNode; 
  }

  bool operator!=(myself iter) 
  { 
    return m_pNode != iter.m_pNode; 
  }

  Node &operator*()
  {
    return *m_pNode;
  }

  myself operator++()
  {
    bool r = 0;
    Node *pNode = m_pNode;
    pNode->setPrinted(m_printed ? 0 : 1);
    while (r == 0)
    {
      pNode = preorden(pNode, r);
    }
    m_pNode = pNode;
    return *this;
  }

  Node *preorden(Node *pNode, bool &r)
  {
    if (pNode->getPrinted() == m_printed)
    {
      r = 1;
      pNode->setPrinted(m_printed ? 0 : 1);
      return pNode;
    }
    if (pNode->getChild(0) != nullptr && pNode->getChild(0)->getPrinted() == m_printed)
    {
      return preorden(pNode->getChild(0), r);
    }
    if (pNode->getChild(1) != nullptr && pNode->getChild(1)->getPrinted() == m_printed)
    {
      return preorden(pNode->getChild(1), r);
    }
    return pNode->getParent();
  }
};

template <typename Container>
class binary_tree_print_iterator
{

public:
  typedef typename Container::Node Node;
  typedef binary_tree_print_iterator<Container> myself;

private:
  Container *m_pContainer;
  Node *m_pNode;
  bool m_printed;

public:
  binary_tree_print_iterator(Container *pContainer, Node *pNode, bool printed)
      : m_pContainer(pContainer), m_pNode(pNode), m_printed(printed) {}
  binary_tree_print_iterator(myself &other)
      : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode), m_printed(other.m_printed) {}
  binary_tree_print_iterator(myself &&other) // Move constructor
  {
    m_pContainer = move(other.m_pContainer);
    m_pNode = move(other.m_pNode);
  }
  myself operator=(myself &iter)
  {
    m_pContainer = move(iter.m_pContainer);
    m_pNode = move(iter.m_pNode);
    return *(myself *)this; // Pending static_cast?
  }

public:
  bool operator==(myself iter) { return m_pNode == iter.m_pNode; }
  bool operator!=(myself iter) { return !(*this == iter); }

  Node &operator*()
  {
    return *m_pNode;
  }

  myself operator++()
  {
    bool r = 0;
    Node *pNode = m_pNode;
    pNode->setPrinted(m_printed ? 0 : 1);
    while (r == 0)
    {
      pNode = printAsTree(pNode, r);
    }
    m_pNode = pNode;
    return *this;
  }

  Node *printAsTree(Node *pNode, bool &r)
  {
    if (pNode->getChild(1) != nullptr && pNode->getChild(1)->getPrinted() == m_printed)
    {
      return printAsTree(pNode->getChild(1), r);
    }
    if (pNode->getPrinted() == m_printed)
    {
      r = 1;
      pNode->setPrinted(m_printed ? 0 : 1);
      return pNode;
    }
    if (pNode->getChild(0) != nullptr && pNode->getChild(0)->getPrinted() == m_printed)
    {
      return printAsTree(pNode->getChild(0), r);
    }
    return pNode->getParent();
  }
};

#endif
