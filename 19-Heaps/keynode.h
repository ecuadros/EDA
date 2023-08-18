@@ -46,6 +46,11 @@ class KeyNode {
    bool operator>(const KeyNode<T, V>& other) const { 
        return m_key > other.m_key;
    }

    constexpr explicit operator value_type() const noexcept { // since C++14
        return m_key;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const KeyNode<T, V>& node) {
        os << "Key: " << node.m_key << ", Value: " << node.m_value;
        return os;
    }
};

#endif