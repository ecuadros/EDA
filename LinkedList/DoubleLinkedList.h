#ifndef __DOUBLELINKEDLIST_H__
#define __DOUBLELINKEDLIST_H__
#include <utility>
#include <algorithm>
#include <cassert>
using namespace std;

template <typename KeyType, typename ValueType>
class NodeDoubleLinkedList
{
private:
    using Node = NodeDoubleLinkedList<KeyType, ValueType>;

    KeyNode<KeyType, ValueType> value;
    Node *next_ptr;
    Node *prev_ptr;

public:
    NodeDoubleLinkedList(const KeyNode<KeyType, ValueType> &value,
                         Node *next_ptr = nullptr,
                         Node *prev_ptr = nullptr)
        : value(value),
          next_ptr(next_ptr),
          prev_ptr(prev_ptr) {}

    const KeyNode<KeyType, ValueType> &getValue() const
    {
        return value;
    }

    void setNext(Node *next)
    {
        next_ptr = next;
    }

    Node *getNext() const
    {
        return next_ptr;
    }

    void setPrevious(Node *prev)
    {
        prev_ptr = prev;
    }

    Node *getPrevious() const
    {
        return prev_ptr;
    }
};

template <typename KeyType, typename ValueType>
struct AscendingDoubleLinkedList
{
    using T = KeyType;
    using Node = NodeDoubleLinkedList<KeyType, ValueType>;
    using CompareFn = less<T>;
};

template <typename KeyType, typename ValueType>
struct DescendingDoubleLinkedList
{
    using T = KeyType;
    using Node = NodeDoubleLinkedList<KeyType, ValueType>;
    using CompareFn = greater<T>;
};

template <typename KeyType, typename ValueType, typename CompareFn>
class DoubleLinkedList
{
public:
    using Node = NodeDoubleLinkedList<KeyType, ValueType>;

private:
    Node *head;
    Node *tail;

public:
    DoubleLinkedList() : head(nullptr), tail(nullptr) {}

    void insert(const KeyNode<KeyType, ValueType> &value)
    {
        Node *newNode = new Node(value);
        if (!head || CompareFn()(value.getKey(), head->getValue().getKey()))
        {
            newNode->setNext(head);
            newNode->setPrevious(nullptr);
            if (head)
                head->setPrevious(newNode);
            head = newNode;
            if (!tail)
                tail = newNode;
        }
        else
        {
            Node *current = head;
            while (current->getNext() && !CompareFn()(value.getKey(), current->getNext()->getValue().getKey()))
            {
                current = current->getNext();
            }
            newNode->setNext(current->getNext());
            newNode->setPrevious(current);
            if (current->getNext())
                current->getNext()->setPrevious(newNode);
            current->setNext(newNode);
            if (!newNode->getNext())
                tail = newNode;
        }
    }

    void printList() const
    {
        Node *current = head;
        while (current)
        {
            std::cout << "Key: " << current->getValue().getKey() << ", Value: " << current->getValue().getValue() << std::endl;
            current = current->getNext();
        }
    }

    class IteratorForward
    {
    private:
        Node *current;

    public:
        IteratorForward(Node *node) : current(node) {}

        const KeyNode<KeyType, ValueType> &operator*()
        {
            return current->getValue();
        }

        IteratorForward &operator++()
        {
            if (current)
            {
                current = current->getNext();
            }
            return *this;
        }

        bool operator==(const IteratorForward &other) const
        {
            return current == other.current;
        }

        bool operator!=(const IteratorForward &other) const
        {
            return current != other.current;
        }
    };

    IteratorForward begin()
    {
        return IteratorForward(head);
    }

    IteratorForward end()
    {
        return IteratorForward(nullptr);
    }

    class BackwardIterator
    {
    private:
        Node *current;

    public:
        BackwardIterator(Node *node) : current(node) {}

        const KeyNode<KeyType, ValueType> &operator*()
        {
            return current->getValue();
        }

        BackwardIterator &operator--()
        {
            if (current)
            {
                current = current->getPrevious();
            }
            return *this;
        }

        bool operator==(const BackwardIterator &other) const
        {
            return current == other.current;
        }

        bool operator!=(const BackwardIterator &other) const
        {
            return current != other.current;
        }
    };

    BackwardIterator rbegin()
    {
        return BackwardIterator(tail);
    }

    BackwardIterator rend()
    {
        return BackwardIterator(nullptr);
    }
};
#endif