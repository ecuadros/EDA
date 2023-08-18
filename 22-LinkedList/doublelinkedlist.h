#include "linkedlist.h"

template <typename K, typename V>
class KeyNode
{
public:
    K m_key;
    V m_value;

    KeyNode(K key, V value) : m_key(key), m_value(value) {}
};

template <typename K, typename V>
class NodeLinkedList : public KeyNode<K, V>
{
public:
    NodeLinkedList(K key, V value) : KeyNode<K, V>(key, value) {}
};

template <typename K, typename V>
class LinkedList
{
public:
    template <typename T>
    class NodeDLL : public NodeLinkedList<T, V>
    {
    public:
        NodeDLL<T>(T key, V value) : NodeLinkedList<T, V>(key, value), m_pPrev(nullptr), m_pNext(nullptr) {}

        NodeDLL<T> *m_pPrev;
        NodeDLL<T> *m_pNext;
    };

    using Type = K;
    using value_type = K;
    using LinkedValueType = V;

    template <typename _K, typename _V>
    struct DLLTrait
    {
        using value_type = _K;
        using LinkedValueType = _V;
        using Node = NodeDLL<_K, _V>;
    };

    template <typename Container>
    class linkedlist_backward_iterator;

    template <typename Container>
    class general_iterator
    {
    public:
        typedef typename Container::Node Node;
        typedef class general_iterator<Container> Parent;

        Container *m_pContainer;
        Node *m_pNode;

    public:
        general_iterator(Container *pContainer, Node *pNode) : m_pContainer(pContainer), m_pNode(pNode) {}
        general_iterator(const Parent &other) : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode) {}

        bool operator==(const Parent &iter)
        {
            return m_pNode == iter.m_pNode;
        }

        bool operator!=(const Parent &iter)
        {
            return !(*this == iter);
        }

        Node &operator*()
        {
            return *m_pNode;
        }
    };

    template <typename Container>
    class linkedlist_backward_iterator : public general_iterator<Container>
    {
    public:
        typedef typename Container::Node Node;
        typedef class general_iterator<Container> Parent;
        typedef linkedlist_backward_iterator<Container> myself;

        linkedlist_backward_iterator(Container *pContainer, Node *pNode) : Parent(pContainer, pNode) {}
        linkedlist_backward_iterator(const myself &other) : Parent(other) {}

        myself operator++()
        {
            Parent::m_pNode = Parent::m_pNode->m_pPrev;
            return *this;
        }

        bool operator==(const myself &iter)
        {
            return Parent::m_pNode == iter.m_pNode;
        }

        bool operator!=(const myself &iter)
        {
            return !(*this == iter);
        }
    };

    using Node = typename DLLTrait<K, V>::Node;
    using iterator = linkedlist_backward_iterator<LinkedList<K, V>>;

    Node *m_pHead;
    Node *m_pTail;

public:
    LinkedList() : m_pHead(nullptr), m_pTail(nullptr) {}

    void push_front(K key, V value)
    {
        Node *newNode = new Node(key, value);
        if (m_pHead == nullptr)
        {
            m_pHead = newNode;
            m_pTail = newNode;
        }
        else
        {
            newNode->m_pNext = m_pHead;
            m_pHead->m_pPrev = newNode;
            m_pHead = newNode;
        }
    }

    iterator begin()
    {
        return iterator(this, m_pHead);
    }

    iterator end()
    {
        return iterator(this, nullptr);
    }
};

int main()
{
    LinkedList<int, std::string> myList;

    myList.push_front(1, "One");
    myList.push_front(2, "Two");
    myList.push_front(3, "Three");

    for (auto it = myList.begin(); it != myList.end(); ++it)
    {
        std::cout << it->m_key << ": " << it->m_value << std::endl;
    }

    return 0;
}