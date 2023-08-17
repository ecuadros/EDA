#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#include <algorithm>
#include <utility>

template <typename Container, class IteratorBase>
class general_iterator
{
public:
    typedef typename Container::Node Node;
    typedef typename Node::Type Type;
    typedef general_iterator<Container, IteratorBase> myself;

protected:
    Container* m_pContainer;
    Node* m_pNode;

public:
    general_iterator(Container* pContainer, Node* pNode)
        : m_pContainer(pContainer), m_pNode(pNode) {}
    general_iterator(const myself& other)
        : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode) {}
    general_iterator(myself&& other) // Move constructor
    {
        m_pContainer = std::move(other.m_pContainer);
        m_pNode = std::move(other.m_pNode);
    }
    IteratorBase operator=(const IteratorBase& iter)
    {
        m_pContainer = iter.m_pContainer;
        m_pNode = iter.m_pNode;
        return *(IteratorBase*)this; // Pending static_cast?
    }

    bool operator==(const IteratorBase& iter) { return m_pNode == iter.m_pNode; }
    bool operator!=(const IteratorBase& iter) { return !(*this == iter); }
    Type& operator*() { return m_pNode->getDataRef(); }
};

#endif