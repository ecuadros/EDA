#include <cassert>
#include <iostream>

template <typename T>
class NodeLinkedList;

template <typename T>
class ForwardIterator {
public:
    typedef NodeLinkedList<T> Node;

private:
    Node* m_pNode;

public:
    ForwardIterator(Node* pNode) : m_pNode(pNode) {}
    ForwardIterator(const ForwardIterator& other) : m_pNode(other.m_pNode) {}

    ForwardIterator& operator++() {
        m_pNode = m_pNode->getNext();
        return *this;
    }

    bool operator==(const ForwardIterator& other) const {
        return m_pNode == other.m_pNode;
    }

    bool operator!=(const ForwardIterator& other) const {
        return !(*this == other);
    }

    T& operator*() {
        return m_pNode->getData();
    }
};

template <typename T>
class NodeLinkedList {
public:
    typedef T Type;
    typedef NodeLinkedList<T> Node;
    typedef ForwardIterator<T> Iterator;

private:
    T m_data;
    Node* m_pNext;

public:
    NodeLinkedList(T data, Node* pNext = nullptr) : m_data(data), m_pNext(pNext) {}

    T getData() const {
        return m_data;
    }

    Node* getNext() const {
        return m_pNext;
    }

    void setNext(Node* pNext) {
        m_pNext = pNext;
    }

    Iterator begin() {
        return Iterator(this);
    }

    Iterator end() {
        return Iterator(nullptr);
    }
};

int main() {
    NodeLinkedList<int> node1(1);
    NodeLinkedList<int> node2(2);
    NodeLinkedList<int> node3(3);

    node1.setNext(&node2);
    node2.setNext(&node3);

    for (auto it = node1.begin(); it != node1.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}