#ifndef KEYNODE_H
#define KEYNODE_H

template <typename KeyType, typename ValueType>
class KeyNode
{
public:
    KeyNode(const KeyType &key, const ValueType &value)
        : key(key), value(value) {}

    // Copy constructor
    KeyNode(const KeyNode &other)
        : key(other.key), value(other.value) {}

    // Move constructor
    KeyNode(KeyNode &&other) noexcept
        : key(std::move(other.key)), value(std::move(other.value)) {}

    KeyNode() {}

    // Copy assignment operator
    KeyNode &operator=(const KeyNode &other)
    {
        if (this != &other)
        {
            key = other.key;
            value = other.value;
        }
        return *this;
    }

    const KeyType &getKey() const { return key; }
    const ValueType &getValue() const { return value; }

    bool operator<(const KeyNode &other) const { return key < other.key; }
    bool operator>(const KeyNode &other) const { return key > other.key; }

private:
    KeyType key;
    ValueType value;
};

#endif // KEYNODE_H
