#include "LinkedList.h"
using namespace std;

template <typename KeyType, typename ValueType, typename CompareFn>
class HashTable
{
public:
    using Node = typename LinkedList<KeyType, ValueType, CompareFn>::Node;

private:
    std::vector<Node *> buckets;
    size_t tableSize;
    size_t capacity;

public:
    HashTable(size_t capacity) : buckets(capacity, nullptr), tableSize(0), capacity(capacity) {}

    // Get the value associated with a key in the hash table
    ValueType *get(const KeyType &key)
    {
        size_t index = hash(key) % capacity;
        Node *current = buckets[index];
        while (current)
        {
            if (CompareFn()(key, current->getValue().getKey()) == 0)
            {
                return &(current->getValue().getValue());
            }
            current = current->getNext();
        }
        return nullptr; // Key not found
    }

    void insert(const KeyNode<KeyType, ValueType> &value)
    {
        size_t index = hash(value.getKey()) % capacity;
        Node *newNode = new Node(value);
        newNode->setNext(buckets[index]);
        buckets[index] = newNode;
        tableSize++;
    }

    // Check if a key exists in the hash table
    bool contains(const KeyType &key)
    {
        return get(key) != nullptr;
    }

    // Remove a key-value pair from the hash table
    void remove(const KeyType &key)
    {
        size_t index = hash(key) % capacity;
        Node *current = buckets[index];
        Node *previous = nullptr;

        while (current && CompareFn()(key, current->getValue().getKey()) != 0)
        {
            previous = current;
            current = current->getNext();
        }

        if (current)
        {
            if (previous)
            {
                previous->setNext(current->getNext());
            }
            else
            {
                buckets[index] = current->getNext();
            }
            delete current;
            tableSize--;
        }
    }

    // Calculate the hash value for a key
    size_t hash(const KeyType &key)
    {
        return std::hash<KeyType>()(key);
    }

    // Get the size of the hash table
    size_t size() const
    {
        return tableSize;
    }

    // Check if the hash table is empty
    bool empty() const
    {
        return tableSize == 0;
    }

    void printTable()
    {
        for (size_t i = 0; i < capacity; ++i)
        {
            std::cout << "Bucket " << i << ": ";
            Node *current = buckets[i];
            while (current)
            {
                std::cout << "(" << current->getValue().getKey() << ", " << current->getValue().getValue() << ") ";
                current = current->getNext();
            }
            std::cout << std::endl;
        }
    }
};