#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__
#include <utility>
#include <algorithm>
#include <cassert>
using namespace std;

template <typename KeyType, typename ValueType>
class NodeLinkedList
{
private:
  using Node = NodeLinkedList<KeyType, ValueType>;

  KeyNode<KeyType, ValueType> value;
  Node *next_ptr;

public:
  NodeLinkedList(const KeyNode<KeyType, ValueType> &value,
                 Node *next_ptr = nullptr)
      : value(value),
        next_ptr(next_ptr) {}

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
};

template <typename KeyType, typename ValueType>
struct AscendingLinkedList
{
  using T = KeyType;
  using Node = NodeLinkedList<KeyType, ValueType>;
  using CompareFn = less<T>;
};

template <typename KeyType, typename ValueType>
struct DescendingLinkedList
{
  using T = KeyType;
  using Node = NodeLinkedList<KeyType, ValueType>;
  using CompareFn = greater<T>;
};

template <typename KeyType, typename ValueType, typename CompareFn>
class LinkedList
{
public:
  using Node = NodeLinkedList<KeyType, ValueType>;

private:
  Node *head;

public:
  LinkedList() : head(nullptr) {}

  // Get the value associated with a key in the linked list
  ValueType *get(const KeyType &key)
  {
    Node *current = head;
    while (current)
    {
      if (!head || CompareFn()(key, current->getValue().getKey()) < 0)
      {
        return &(current->getValue().getValue());
      }
      current = current->getNext();
    }
    return nullptr; // Key not found
  }

  void insert(const KeyNode<KeyType, ValueType> &value)
  {
    Node *newNode = new Node(value);
    if (!head || CompareFn()(value.getKey(), head->getValue().getKey()))
    {
      newNode->setNext(head);
      head = newNode;
    }
    else
    {
      Node *current = head;
      while (current->getNext() && !CompareFn()(value.getKey(), current->getNext()->getValue().getKey()))
      {
        current = current->getNext();
      }
      newNode->setNext(current->getNext());
      current->setNext(newNode);
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

  Node *operator[](size_t position)
  {
    size_t index = 0;
    Node *current = head;

    while (current)
    {
      if (index == position)
      {
        return current;
      }
      current = current->getNext();
      index++;
    }

    return nullptr; // Return nullptr if position is out of range
  }

  friend std::ostream &operator<<(std::ostream &os, const LinkedList &list)
  {
    Node *current = list.head;
    while (current)
    {
      os << "Key: " << current->getValue().getKey() << ", Value: " << current->getValue().getValue() << std::endl;
      current = current->getNext();
    }
    return os;
  }

  friend std::istream &operator>>(std::istream &is, LinkedList &list)
  {
    KeyType key;
    ValueType value;

    std::cout << "Enter key: ";
    is >> key;
    std::cout << "Enter value: ";
    is >> value;

    KeyNode<KeyType, ValueType> keyValue(key, value);
    list.insert(keyValue);

    return is;
  }

  void pop(size_t position)
  {
    if (position == 0)
    {
      Node *temp = head;
      head = head->getNext();
      delete temp;
    }
    else
    {
      size_t index = 0;
      Node *current = head;
      Node *previous = nullptr;

      while (current && index < position)
      {
        previous = current;
        current = current->getNext();
        index++;
      }

      if (current)
      {
        previous->setNext(current->getNext());
        delete current;
      }
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
};
#endif