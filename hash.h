#ifndef __HASH_H__
#define __HASH_H__
#include <iostream>
#include <functional>
#include <vector>

using namespace std;

#define INTERNAL_ARR_DEFAULT_SIZE 16
#define INTERNAL_ARR_MAX 0.75f

template <typename Traits>
class HashMap
{
public:
    using value_type = typename Traits::value_type;
    using KeyType = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using Node = typename Traits::Node;
    using CompareFn = typename Traits::CompareFn;
    using myself = CArray<Traits>;

private:
    vector<vector<Node>> arr = vector<vector<Node>>(INTERNAL_ARR_DEFAULT_SIZE);
    string name;
    size_t count = 0;

public:
    HashMap() : name("HashMap") {}
    HashMap(const string &name) : name(name) {}
    ~HashMap()
    {
        cout << "Destroying " << name << endl;
    }

    void insert(const KeyType &key, const LinkedValueType &value)
    {
        size_t idx = getIdx(key);
        arr[idx].push_back(Node(key, value));
        resizeIfNeeded();
        count++;
    }

    bool has(const KeyType &key)
    {
        size_t idx = getIdx(key);
        for (Node &node : arr[idx])
        {
            if (node.m_key == key)
            {
                return true;
            }
        }
        return false;
    }

    LinkedValueType &find(const KeyType &key)
    {
        size_t idx = getIdx(key);
        for (Node &node : arr[idx])
        {
            if (node.m_key == key)
            {
                return node.getValueRef();
            }
        }
        throw out_of_range("Value not in the hashmap");
    }

    LinkedValueType &operator[](const KeyType &key)
    {
        return find(key);
    }

    void print(ostream &os)
    {
        for (vector<Node> &sub : arr)
        {
            for (Node &el : sub)
            {
                os << "[" << el.getDataRef() << "]->(" << el.getValueRef() << ")" << endl;
            }
        }
    }

    void read(istream &is)
    {
        while (!is.eof())
        {
            KeyType key;
            LinkedValueType val;
            is >> key >> val;
            insert(key, val);
        }
    }

    template <typename T>
    friend ostream &operator<<(ostream &os, HashMap<T> &obj);

    template <typename T>
    friend istream &operator>>(istream &is, HashMap<T> &obj);

private:
    size_t getIdx(const KeyType &key)
    {
        size_t h = hash<KeyType>{}(key);
        size_t idx = h % arr.capacity();
        return idx;
    }

    void resizeIfNeeded()
    {
        size_t maxSize = arr.capacity() * INTERNAL_ARR_MAX;
        if (count > maxSize)
        {
            vector<vector<Node>> temp = arr;
            arr.clear();
            arr.resize(arr.capacity() * 2);
            for (vector<Node> &sub : temp)
            {
                for (Node& el : sub)
                {
                    KeyType k = el.getDataRef();
                    LinkedValueType v = el.getValueRef();
                    size_t idx = getIdx(k);
                    arr[idx].push_back(el);
                }
            }
        }
    }
};

template <typename T>
ostream &operator<<(ostream &os, HashMap<T> &obj)
{
    obj.print(os);
    return os;
}

template <typename T>
istream &operator>>(istream &is, HashMap<T> &obj)
{
    obj.read(is);
    return is;
}

#endif